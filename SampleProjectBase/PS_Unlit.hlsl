Texture2D myTexture : register(t0); // テクスチャー
SamplerState mySampler : register(s0); // サンプラー

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

// マテリアル
struct Material
{
    float4 diffuse; // 色
    float4 ambient; // 環境光
    float4 specular; // 鏡面反射
    float4 emissive; // 自発光
    float shininess; // 光沢
    int isTextureEnable;   // テクスチャ使用
    float2 dummy;
};

cbuffer BufMaterial : register(b0)
{
    Material material;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    // テクスチャが使用されているなら
    if (material.isTextureEnable != 0)
    {
        color *= myTexture.Sample(mySampler, pin.uv);
    }
    
	return color;
}