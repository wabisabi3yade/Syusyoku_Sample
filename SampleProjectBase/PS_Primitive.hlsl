struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

// ���_�F��`�悷��
float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
	return color;
}