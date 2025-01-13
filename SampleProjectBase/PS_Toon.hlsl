struct PS_IN
{
   // スクリーン座標
    float4 pos : SV_POSITION0;
     // 頂点カラー
    float4 color : COLOR0;
    // uv座標
    float2 uv : TEXCOORD0;
    // 法線ベクトル
    float3 normal : NORMAL0;
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

// マテリアル
cbuffer BufMaterial : register(b0)
{
    Material matBuf;
};

// ライト
cbuffer BufLight : register(b1)
{
    DirectionLight dirL;
};

Texture2D tex : register(t0);
Texture2D rampTex : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float3 N = normalize(pin.normal);
    float3 L = normalize(dirL.direction.xyz);
    L = -L;
    
    float diffuse = saturate(dot(N, L));
    
#if 0
    float2 rampUV = float2(diffuse * 0.98f + 0.01f, 0.5f);
    diffuse = rampTex.Sample(samp, rampUV).r;
#else
    diffuse *= 2.0f;
    diffuse += 0.3f;
    diffuse = floor(diffuse);
    diffuse *= 0.5f;
    //diffuse *= matBuf.diffuse.rgb;
    float3 diffuse3 = diffuse + matBuf.ambient.rgb;
    diffuse3 = saturate(diffuse3);
    diffuse3 += dirL.ambiemtColor.rgb;
#endif
    float4 texColor = tex.Sample(samp, pin.uv);
    color.rgb = texColor.rgb * diffuse3.rgb;
    color.rgb = saturate(color.rgb);
   
    color.a = matBuf.diffuse.a;
    
    return color;
}