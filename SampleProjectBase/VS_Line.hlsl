struct VS_INPUT
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float3 color : COLOR0; // ���_�F
};

// �s�N�Z���V�F�[�_�[�ɓn��
struct VS_OUTPUT
{
    // �X�N���[�����W
    float4 pos : SV_POSITION0;
     // ���_�J���[
    float4 color : COLOR0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT vout;
    
    vout.pos = float4(vin.pos, 0.0f);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    vout.color = float4(vin.color, 1.0f);
    
	return vout;
}