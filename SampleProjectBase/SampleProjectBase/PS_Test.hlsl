Texture2D myTexture : register(t0); // �e�N�X�`���[
SamplerState mySampler : register(s0); // �T���v���[

// �}�e���A��
struct Material
{
    float4 diffuse; // �F
    float4 ambient; // ����
    float4 specular; // ���ʔ���
    float4 emissive; // ������
    float shininess; // ����
};

// �}�e���A��
cbuffer Material : register(b1)
{
    Material material;
}

struct PS_INPUT
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

float4 main(PS_INPUT pin) : SV_TARGET
{
    float4 color = pin.color;
    //color *= material.diffuse;
    
    
    
    return color;
}