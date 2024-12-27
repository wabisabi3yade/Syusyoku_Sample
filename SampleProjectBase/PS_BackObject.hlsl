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
    float4 screenPos : POSITION1;
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
    
    float len = length(pin.worldPos.xyz - camPos.xyz);
    float alpha = color.a * saturate(len * 0.2f);
    
    // スクリーンの座標に応じて、
    // ディザパターンを適用する位置を計算
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV *= float2(SCREEN_WIDTH, SCREEN_HEIGHT) / 4.0f;
    
    // ディザパターンの取得
    float dither = ditherTex.Sample(mySampler, screenUV).r;
    if(alpha < dither)
    {
        // 透明度0で見えないけどそこにある、ではなく完全に表示を行わない
        discard;
    }
    
    return color;
}