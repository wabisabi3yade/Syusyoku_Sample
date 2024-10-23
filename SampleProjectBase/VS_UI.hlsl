// Slot0 ���e�s��
cbuffer Projection : register(b0)
{
    // ���e�s��
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float4 color : COLOR0; // ���_�F
    float2 uv : TEXCOORD0; // uv���W
};

// �s�N�Z���V�F�[�_�[�ɓn��
struct VS_OUTPUT
{
    // �X�N���[�����W
    float4 pos : SV_POSITION0;
     // ���_�J���[
    float4 color : COLOR0;
    // uv���W
    float2 uv : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;
    
    output.pos = float4(vin.pos.xyz, 1.0f);
    output.pos = mul(output.pos, projection);
    
    output.color = vin.color;
    output.uv = vin.uv;
    
	return output;
}