struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // ���_�F
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

float4 main(PS_IN pin) : SV_TARGET
{
    return pin.color;
}