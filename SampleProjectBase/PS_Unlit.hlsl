Texture2D myTexture : register(t0); // �e�N�X�`���[
SamplerState mySampler : register(s0); // �T���v���[

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

// �}�e���A��
struct Material
{
    float4 diffuse; // �F
    float4 ambient; // ����
    float4 specular; // ���ʔ���
    float4 emissive; // ������
    float shininess; // ����
    int isTextureEnable;   // �e�N�X�`���g�p
    float2 dummy;
};

//// ���C�g�̊�b�p�����[�^
//struct LightBase
//{
//    float4 color; // �F
//    float3 position; // ���W  
//    float dummy;
//};

//// �f�B���N�V�������C�g
//struct DirectionLight
//{
//    LightBase base;
//    float4 ambiemtColor; // ���F
//    float3 direction; // ����
//    float dummy;
//};

//// �|�C���g���C�g
//struct PointLight
//{
//    LightBase base;
//    float range; // �͈�
//    float3 dummy;
//};

//// �X�|�b�g���C�g
//struct SpotLight
//{
//    LightBase base;
//    float3 direction; // ����
//    float distance; // ����
//    float angleRange; // �p�x�͈�
//    float3 dummy;
//};

cbuffer BufMaterial : register(b0)
{
    Material material;
}

//// ���C�g
//cbuffer BufLight : register(b1)
//{
//    DirectionLight dirL;
//    PointLight pointL;
//    SpotLight spotL;
//};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    // �e�N�X�`�����g�p����Ă���Ȃ�
    if (material.isTextureEnable != 0)
    {
        color *= myTexture.Sample(mySampler, pin.uv);
    }
    
	return color;
}