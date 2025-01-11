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
    
    float2 shadowMapUV = pin.lightSpacePos.xy / pin.lightSpacePos.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float zInLVP = pin.lightSpacePos.z;
    
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        float2 shadowValue = depthTex.Sample(samp, shadowMapUV).xy;
        
        if (zInLVP > shadowValue.r && zInLVP <= 1.0f)
        {
            float depth_sq = shadowValue.x * shadowValue.x;
            
            float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);
            float md = zInLVP - shadowValue.x;
            
            float lit_factor = variance / (variance + md * md);
            
            float3 shadowColor = color.xyz * 0.5f;
            
            color.xyz = lerp(shadowColor, color.xyz, lit_factor);
        }

    }
    
    return color;
}