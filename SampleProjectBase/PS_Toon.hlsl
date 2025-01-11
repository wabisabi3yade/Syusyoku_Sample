struct PS_IN
{
   // �X�N���[�����W
    float4 pos : SV_POSITION0;
     // ���_�J���[
    float4 color : COLOR0;
    // uv���W
    float2 uv : TEXCOORD0;
    // �@���x�N�g��
    float3 normal : NORMAL0;
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

// �}�e���A��
cbuffer BufMaterial : register(b0)
{
    Material matBuf;
};

// ���C�g
cbuffer BufLight : register(b1)
{
    DirectionLight dirL;
};

Texture2D tex : register(t0);
Texture2D rampTex : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float3 N = normalize(pin.normal);
    float3 L = normalize(dirL.direction.xyz);
    L = -L;
    
    float diffuse = saturate(dot(N, L));
    
#if 0
    float2 rampUV = float2(diffuse * 0.98f + 0.01f, 0.5f);
    diffuse = rampTex.Sample(samp, rampUV).r;
#else
    diffuse *= 2.0f;
    diffuse += 0.3f;
    diffuse = floor(diffuse);
    diffuse *= 0.5f;
    //diffuse *= matBuf.diffuse.rgb;
    float3 diffuse3 = diffuse + matBuf.ambient.rgb;
    diffuse3 = saturate(diffuse3);
    diffuse3 += dirL.ambiemtColor.rgb;
#endif
    float4 texColor = tex.Sample(samp, pin.uv);
    color.rgb = texColor.rgb * diffuse3.rgb;
    color.rgb = saturate(color.rgb);
   
    color.a = matBuf.diffuse.a;
    
    return color;
}