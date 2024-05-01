// Slot0 ���W�ϊ�
cbuffer cbTransform : register(b0) {
    matrix Transform;
};

// Slot1 �r���[�ϊ�
cbuffer cbProjection : register(b1) { 
    matrix View;
};

// Slot2 ���e�ϊ�
cbuffer cbProjection : register(b2) { 
    matrix Projection;
};

struct VS_INPUT
{
    float3 Pos : POSITION; // ���_���W�i���f�����W�n�j
    float4 Col : COLOR; // ���_�F
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Col : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // �e�s����v�Z����
    float4 pos = float4(input.Pos, 1.0);
    pos = mul(pos, Transform);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.Pos = pos;

    output.Col = input.Col;

    return output;
}