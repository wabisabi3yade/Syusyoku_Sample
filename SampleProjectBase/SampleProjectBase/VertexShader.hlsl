// Slot0 ���W�ϊ�
cbuffer cbTransform : register(b0) {
    matrix transform;
};

// Slot1 �r���[�ϊ�
cbuffer cbView : register(b1) { 
    matrix view;
};

// Slot2 ���e�ϊ�
cbuffer cbProjection : register(b2) { 
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float4 col : COLOR; // ���_�F
    float2 uv : TEXCOORD0;   // uv���W
    float3 normal : NORMAL0;    // �@���x�N�g��
};

// �s�N�Z���V�F�[�_�[�ɓn��
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // �e�s����v�Z����
    float4 pos = float4(input.pos, 1.0);
    pos = mul(pos, transform);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;


    output.col = input.col;
    output.uv = input.uv;

    return output;
}