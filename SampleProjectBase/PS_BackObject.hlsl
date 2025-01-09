#define PI (3.1415926f) // PBR
#define SCREEN_WIDTH (1280.0f) // �E�B���h�E��
#define SCREEN_HEIGHT (720.0f) // �E�B���h�E�c
    
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
    float4 lightSpacePos : POSITION1;
};

struct Material
{
    float4 diffuse; // �F
    float4 ambient; // ����
    float4 specular; // ���ʔ���
    float4 emissive; // ������
    float shininess; // ����
    int isTextureEnable; // �e�N�X�`���t���O
    float2 dummy;
};

// ���C�g�̊�b�p�����[�^
struct LightBase
{
    float4 color; // �F
    float3 position; // ���W  
    float dummy;
};

// �f�B���N�V�������C�g
struct DirectionLight
{
    LightBase base;
    float4 ambiemtColor; // ���F
    float3 direction; // ����
    float dummy;
};

// �|�C���g���C�g
struct PointLight
{
    LightBase base;
    float range; // �͈�
    float3 dummy;
};

// �X�|�b�g���C�g
struct SpotLight
{
    LightBase base;
    float3 direction; // ����
    float distance; // ����
    float angleRange; // �p�x�͈�
    float3 dummy;
};

// �}�e���A��
cbuffer MaterialBuffer : register(b0)
{
    Material material;
};

// ���C�g
cbuffer BufLight : register(b1)
{
    DirectionLight dirL;
    PointLight pointL;
    SpotLight spotL;
};

// �J����
cbuffer Camera : register(b2)
{
    float4 camPos;
};

Texture2D myTexture : register(t0); // �e�N�X�`���[
Texture2D ditherTex : register(t1); // �A���t�@�f�B�U�[�e�N�X�`���[
Texture2D depthTex : register(t2); // �[�x�e�N�X�`���[
SamplerState mySampler : register(s0); // �T���v���[

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    // �t�f�B���N�V�������C�g�x�N�g��
    float3 arcLightDir = -dirL.direction;
    float dotNL = saturate(dot(pin.normal.xyz, arcLightDir));
    color.rgb *= dotNL * material.diffuse.rgb * dirL.base.color.rgb;
    color.rgb += dirL.ambiemtColor.rgb * material.diffuse.rgb;
    
    // �e�N�X�`�����g�p����Ă���Ȃ�
    if (material.isTextureEnable != 0)
    {
        color *= myTexture.Sample(mySampler, pin.uv);
    }
    // �����x
    color.a *= material.diffuse.a;
    color = saturate(color);
    
    // �e
   // float2 screenUV = pin.lightSpacePos.xy / pin.lightSpacePos.w;
   // screenUV = screenUV * 0.5f + 0.5f;
   // screenUV.y = 1.0f - screenUV.y;
   // screenUV = saturate(screenUV);
   // float objDepth = pin.lightSpacePos.z / pin.lightSpacePos.w;    
   // float texDepth = depthTex.Sample(mySampler, screenUV).r;
   // if (screenUV.x < 0.0f || screenUV.x > 1.0f ||
   //     screenUV.y < 0.0f || screenUV.y > 1.0f)
   // {
   //     texDepth = 1.0f;
   // }
    
   //color.rgb *= objDepth > texDepth ? 0.5f : 1.0f;
    
    
    float len = length(pin.worldPos.xyz - camPos.xyz);
    float alpha = color.a * saturate(len * 0.2f - 0.3f);
    
    // �X�N���[���̍��W�ɉ����āA
    // �f�B�U�p�^�[����K�p����ʒu���v�Z
    float2 ditherUV = pin.pos.xy / float2(SCREEN_WIDTH, SCREEN_HEIGHT);
    ditherUV = frac(ditherUV * 200.0f);
    
    // �f�B�U�p�^�[���̎擾
    float dither = ditherTex.Sample(mySampler, ditherUV).r;
    if (alpha - 1e-6 < dither)
    {
        // �����x0�Ō����Ȃ����ǂ����ɂ���A�ł͂Ȃ����S�ɕ\�����s��Ȃ�
        discard;
    }
    
    return color;
}