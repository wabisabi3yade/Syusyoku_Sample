// Slot0 ���W�ϊ�
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

// �F���
cbuffer InputColor : register(b1)
{
    float4 multiColor; // �|���Z
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
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0; // �@���x�N�g��
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
    
    output.normal = mul(float4(vin.normal, 1.0f), world);

    output.color = multiColor;  // �F���� 
    output.uv = vin.uv;

    return output;
}