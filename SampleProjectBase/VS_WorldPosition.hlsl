// Slot0 ���W�ϊ�
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

// Slot1 ���C�g�r���[�s��
cbuffer WVP : register(b1)
{
    matrix Lworld;
    matrix Lview;
    matrix Lprojection;
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
    // ���C�g�r���[��ԍ��W
    float4 lightSpacePos : POSITION1;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;

    // �e�s����v�Z����
    float4 pos = float4(vin.pos, 1.0);
     // ���[���h���W
    pos = mul(pos, world);
    output.worldPos = pos;
    
     // �X�N���[�����W
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;
    output.color = vin.color;
    output.uv = vin.uv;
    
    // ���C�g�r���[��ԍ��W�����߂�
    output.lightSpacePos = mul(output.worldPos, Lview);
    output.lightSpacePos = mul(output.lightSpacePos, Lprojection);

    float3x3 rotationMatrix = (float3x3)world;
    rotationMatrix[0] = normalize(rotationMatrix[0]);
    rotationMatrix[1] = normalize(rotationMatrix[1]);
    rotationMatrix[2] = normalize(rotationMatrix[2]);
    
    output.normal = mul(vin.normal, rotationMatrix);
    output.normal = normalize(output.normal);

    return output;
}