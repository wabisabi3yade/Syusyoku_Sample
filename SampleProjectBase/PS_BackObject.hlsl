#define PI (3.1415926f) // PBR
#define SCREEN_WIDTH (1280.0f) // ウィンドウ横
#define SCREEN_HEIGHT (720.0f) // ウィンドウ縦
    
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
    float4 lightSpacePos : POSITION1;
};

struct Material
{
    float4 diffuse; // 色
    float4 ambient; // 環境光
    float4 specular; // 鏡面反射
    float4 emissive; // 自発光
    float shininess; // 光沢
    int isTextureEnable; // テクスチャフラグ
    float2 dummy;
};

// ライトの基礎パラメータ
struct LightBase
{
    float4 color; // 色
    float3 position; // 座標  
    float dummy;
};

// ディレクションライト
struct DirectionLight
{
    LightBase base;
    float4 ambiemtColor; // 環境色
    float3 direction; // 方向
    float dummy;
};

// ポイントライト
struct PointLight
{
    LightBase base;
    float range; // 範囲
    float3 dummy;
};

// スポットライト
struct SpotLight
{
    LightBase base;
    float3 direction; // 方向
    float distance; // 距離
    float angleRange; // 角度範囲
    float3 dummy;
};

// マテリアル
cbuffer MaterialBuffer : register(b0)
{
    Material material;
};

// ライト
cbuffer BufLight : register(b1)
{
    DirectionLight dirL;
    PointLight pointL;
    SpotLight spotL;
};

// カメラ
cbuffer Camera : register(b2)
{
    float4 camPos;
};

Texture2D myTexture : register(t0); // テクスチャー
Texture2D ditherTex : register(t1); // アルファディザーテクスチャー
Texture2D depthTex : register(t2); // 深度テクスチャー
SamplerState mySampler : register(s0); // サンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    // 逆ディレクションライトベクトル
    float3 arcLightDir = -dirL.direction;
    float dotNL = saturate(dot(pin.normal.xyz, arcLightDir));
    color.rgb *= dotNL * material.diffuse.rgb * dirL.base.color.rgb;
    color.rgb += dirL.ambiemtColor.rgb * material.diffuse.rgb;
  
    // テクスチャが使用されているなら
    if (material.isTextureEnable != 0)
    {
        color *= myTexture.Sample(mySampler, pin.uv);
    }
    // 透明度
    color.a *= material.diffuse.a;
    color = saturate(color);
    
    // 影    
    float2 shadowMapUV = pin.lightSpacePos.xy / pin.lightSpacePos.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    // ライトビュースクリーン空間からZ値を計算
    float zInLVP = pin.lightSpacePos.z;
    
    if(shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        // シャドウレシーバーを落とす
        // シャドウマップから値をサンプリング
        float2 shadowValue = depthTex.Sample(mySampler, shadowMapUV).xy;
        
        // このピクセルが遮蔽されているか確認
        if(zInLVP > shadowValue.r && zInLVP <= 1.0f)
        {
            // 遮蔽されているならチェビコフの不等式を利用して光が当たる確立を求める
            float depth_sq = shadowValue.x * shadowValue.x;
            
            // このグループの分散具合を求める
            // 分散が大きいほど、varianceの値は大きくなる
            float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);
            
            // このピクセルのライトから見た深度値とシャドウマップの平均の深度値の座を求める
            float md = zInLVP - shadowValue.x;
            
            // 光が届く確率を求める
            float lit_factor = variance / (variance + md * md);
            
            // シャドウカラーを求める
            float3 shadowColor = color.xyz * 0.5f;
            
            // 光が合ったる確率を使って通常カラーとシャドウカラーを線形補間
            color.xyz = lerp(shadowColor, color.xyz, lit_factor);
        }

    }
    
    // アルファディザー
    float len = length(pin.worldPos.xyz - camPos.xyz);
    float alpha = color.a * saturate(len * 0.2f - 0.3f);
    
    // スクリーンの座標に応じて、
    // ディザパターンを適用する位置を計算
    float2 ditherUV = pin.pos.xy / float2(SCREEN_WIDTH, SCREEN_HEIGHT);
    ditherUV = frac(ditherUV * 200.0f);
    
    // ディザパターンの取得
    float dither = ditherTex.Sample(mySampler, ditherUV).r;
    if (alpha - 1e-6 < dither)
    {
        // 透明度0で見えないけどそこにある、ではなく完全に表示を行わない
        discard;
    }
    
    return color;
}