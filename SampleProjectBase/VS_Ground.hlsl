// Slot0 ���W�ϊ�
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

cbuffer LightWVP : register(b1)
{
    float4x4 Lworld;
    float4x4 Lview;
    float4x4 LProj;
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
    // ���C�g���猩�����W
    float4 lightSpacePos : POSITION0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;

    // �e�s����v�Z����
    float4 pos = float4(vin.pos, 1.0f);
     // ���[���h���W
    pos = mul(pos, world);
    float4 worldPos = pos;
     // �X�N���[�����W
    pos = mul(pos, view);
    // �v���W�F�N�V�������W
    pos = mul(pos, projection);
    
    // ���C�g��ԍ��W
    output.lightSpacePos = mul(worldPos, Lview);
    output.lightSpacePos = mul(output.lightSpacePos, LProj);
    
    output.pos = pos;
    output.color = vin.color;
    output.uv = vin.uv;

    return output;
}

