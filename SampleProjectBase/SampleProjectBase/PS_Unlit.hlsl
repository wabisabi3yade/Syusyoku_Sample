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
};

// ���C�g�̊�b�p�����[�^
struct LightBase
{
    float3 position;
    float4 color;
};

// �|�C���g���C�g
struct PointLight
{
    LightBase base;
    float range;
};


// �X�|�b�g���C�g
struct SpotLight
{
    LightBase base;
    float distance;
    float rangeAngle;
    float3 direction;
};

cbuffer BufMaterial : register(b0)
{
    Material material;
}

cbuffer BufPoint : register(b1)
{
    PointLight pointLights[30];
}

cbuffer BufSpot : register(b2)
{
    SpotLight spotLights[30];
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    
	return color;
}