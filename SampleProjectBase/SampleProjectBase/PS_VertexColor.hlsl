// �s�N�Z���V�F�[�_�[�ɓn��
struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

// ���_�F��`�悷��
float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
	return color;
}