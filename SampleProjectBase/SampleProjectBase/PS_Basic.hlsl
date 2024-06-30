Texture2D myTexture : register(t0); // �e�N�X�`���[
SamplerState mySampler : register(s0); // �T���v���[

//// �}�e���A��
//struct Material
//{
//    float4 diffuse; // �F
//    float4 ambient; // ����
//    float4 specular; // ���ʔ���
//    float4 emissive; // ������
//    float shininess; // ����
//    bool isTextureEnable; // �e�N�X�`�����g�p���Ă��邩�t���O
//};

//// �}�e���A��
//cbuffer Material : register(b1)
//{
//    Material material;
//}

struct PS_INPUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
   
    // �e�N�X�`�����g�p���Ă���Ȃ�
    color = myTexture.Sample(mySampler, pin.uv);
    color *= pin.color;
    
    return color;
}