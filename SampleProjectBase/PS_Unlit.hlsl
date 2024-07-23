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

cbuffer BufMaterial : register(b0)
{
    Material material;
}

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