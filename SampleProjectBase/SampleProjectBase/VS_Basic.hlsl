// Slot0 ���W�ϊ�
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct Material
{
    float4 diffuse; // �F
    float4 ambient; // ����
    float4 specular; // ���ʔ���
    float4 emissive; // ������
    float shininess; // ����
};
// Slot1 �}�e���A��
cbuffer MaterialBuffer : register(b1)
{
    Material material;
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

    // �e�s����v�Z����
    float4 pos = float4(vin.pos, 1.0);
     // ���[���h���W
    pos = mul(pos, world);
    
     // �X�N���[�����W
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;
    
    // ���_�J���[�ƃ}�e���A���̐F�Ə�Z
    output.color.rgb = vin.color.rgb * material.diffuse.rgb;
    output.color.a = vin.color.a * material.diffuse.a;
  
    output.uv = vin.uv;

    return output;
}