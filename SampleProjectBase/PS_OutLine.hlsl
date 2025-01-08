struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // í∏ì_êF
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

cbuffer Camera : register(b0)
{
    float4 cameraPos;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    float len = length(pin.worldPos.xyz - cameraPos.xyz);
    color.a = color.a * saturate(len * 0.2f);
    
    return color;
}