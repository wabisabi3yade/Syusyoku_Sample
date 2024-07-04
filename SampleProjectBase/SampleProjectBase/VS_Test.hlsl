// Slot0 ���W�ϊ�
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float4 color : COLOR0; // ���_�F
    float2 uv : TEXCOORD0; // uv���W
    float3 normal : NORMAL0; // �@���x�N�g��
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
    // �@���x�N�g��
    float3 normal : NORMAL0;
    // ���[���h���W
    float4 worldPos : POSITION0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT vout;
    
     // �e�s����v�Z����
    float4 pos = float4(vin.pos, 1.0);
     // ���[���h���W
    pos = mul(pos, world);
    vout.worldPos = pos;
    
     // �X�N���[�����W
    pos = mul(pos, view);
    pos = mul(pos, projection);
    vout.pos = pos;
    vout.uv = vin.uv;
    vout.normal = mul(vin.normal, (float3x3) world);
    
    vout.color = vin.color;

    return vout;
}