
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

// Slot0 ���W�ϊ�
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

// Slot1 �}�e���A��
cbuffer MaterialBuffer : register(b1)
{
    Material material;
};

// ���C�g
cbuffer BufLight : register(b2)
{
    DirectionLight dirL;
    //PointLight pointL;
    //SpotLight spotL;
};

struct VS_IN
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
};

// DirectionLight���^����F�̉e�����v�Z����
float4 CalcDirLight(float3 _normal)
{
    float4 color = float4(0, 0, 0, 0);
    float3 arcLightDir = -dirL.direction;
    
    // �@���x�N�g���Ƌt���C�g�x�N�g���̓���
    float dotNL = dot(_normal, arcLightDir);
    
    color = saturate(dotNL * dirL.base.color + dirL.ambiemtColor);
    
    return color;
}

VS_OUTPUT main(VS_IN vin)
{
    VS_OUTPUT output;
    output.pos = float4(vin.pos, 1.0f);
    output.pos = mul(output.pos, world);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

	// ���_�V�F�[�_�[�ŉA�̌v�Z
    float3 N = vin.normal;
    
    float3x3 rotationMatrix = (float3x3) world;
    rotationMatrix[0] = normalize(rotationMatrix[0]);
    rotationMatrix[1] = normalize(rotationMatrix[1]);
    rotationMatrix[2] = normalize(rotationMatrix[2]);
    
    N = mul(N, rotationMatrix);
    output.normal = normalize(N);
    
	// �}�e���A���F
    output.color = material.diffuse;
    // �f�B���N�V�������C�g�ɐF��Z
    output.color.rgb *= CalcDirLight(N).rgb;
   
    output.uv = vin.uv;
    
    return output;
}