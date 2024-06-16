// Slot0 ���W�ϊ�
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

cbuffer UV_OFFSET : register(b1)
{
    float2 uv_offset;
};


struct VS_INPUT
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float4 color : COLOR; // ���_�F
    float2 uv : TEXCOORD0; // uv���W
    float3 normal : NORMAL0; // �@���x�N�g��
};

// �s�N�Z���V�F�[�_�[�ɓn��
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;

    // �e�s����v�Z����
    float4 pos = float4(vin.pos, 1.0);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;


    output.color = vin.color;
    output.uv = vin.uv + uv_offset;

    return output;
}