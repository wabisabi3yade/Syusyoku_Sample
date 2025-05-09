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

struct TexParam
{
    int isTextureEnable; // テクスチャ使用
    float alpha;    // α値
    float2 dummy;
};

cbuffer BufTexEnable : register(b0)
{
    TexParam texParam;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    color.a = texParam.alpha;
    
    // テクスチャが使用されているなら
    if (texParam.isTextureEnable != 0)
    {
        color *= myTexture.Sample(mySampler, pin.uv);
    }
    
    return color;
}