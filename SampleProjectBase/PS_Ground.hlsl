struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float4 lightSpacePos : POSITION0;
};

struct TexParam
{
    int isTextureEnable; // テクスチャ使用
    float alpha; // α値
    float2 dummy;
};

cbuffer BufTexEnable : register(b0)
{
    TexParam texParam;
}

SamplerState samp : register(s0);
Texture2D diffuseTex : register(t0);
Texture2D depthTex : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    color.a = texParam.alpha;
    
    // テクスチャが使用されているなら
    if (texParam.isTextureEnable != 0)
    {
        color *= diffuseTex.Sample(samp, pin.uv);
    }
    
    float2 screenUV = pin.lightSpacePos.xy / pin.lightSpacePos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV.y = 1.0f - screenUV.y;
    
    float objDepth = pin.lightSpacePos.z / pin.lightSpacePos.w;
    
     //screenUV = saturate(screenUV);
    
    float texDepth = depthTex.Sample(samp, screenUV).r;
    
    if (screenUV.x < 0.0f || screenUV.x > 1.0f ||
        screenUV.y < 0.0f || screenUV.y > 1.0f)
    {
        texDepth = 1.0f;
    }
    
    color.rgb *= objDepth > texDepth ? 0.5f : 1.0f;
    
    return color;
}