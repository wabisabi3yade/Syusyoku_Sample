Texture2D myTexture : register(t0); // テクスチャー
SamplerState mySampler : register(s0); // サンプラー

// マテリアル
struct Material
{
    float4 diffuse; // 色
    float4 ambient; // 環境光
    float4 specular; // 鏡面反射
    float4 emissive; // 自発光
    float shininess; // 光沢
};

// マテリアル
cbuffer Material : register(b1)
{
    Material material;
}

struct PS_INPUT
{
   // スクリーン座標
    float4 pos : SV_POSITION0;
     // 頂点カラー
    float4 color : COLOR0;
    // uv座標
    float2 uv : TEXCOORD0;
    // 法線ベクトル
    float3 normal : NORMAL0;
    // ワールド座標
    float4 worldPos : POSITION0;
};

float4 main(PS_INPUT pin) : SV_TARGET
{
    float4 color = pin.color;
    //color *= material.diffuse;
    
    
    
    return color;
}