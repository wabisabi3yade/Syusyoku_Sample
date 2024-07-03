
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

// �f�B���N�V�������C�g
struct DirectionLight
{
    LightBase base;
    float4 direction;
};

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

// �f�B���N�V�������C�g
cbuffer BufDirectionL : register(b2)
{
    DirectionLight directionL;
};

struct VS_INPUT
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float2 uv : TEXCOORD0; // uv���W
    float4 color : COLOR0; // ���_�F
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

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;
    output.pos = float4(vin.pos, 1.0f);
    output.pos = mul(output.pos, world);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

	// ���_�V�F�[�_�[�ŉA�̌v�Z
    float3 N = vin.normal;
    float3 L = directionL.direction.xyz;
	// �v�Z�O�Ɏ��������
    N = mul(N, (float3x3) world);
    output.normal = N;
    
    L = -L;
	// ���K��
    N = normalize(N);
    L = normalize(L);
	// �A�v�Z
    float diffuse = saturate(dot(N, L));
	// �F����
    output.color = float4(diffuse + directionL.base.color.rgb, 1.0f) + material.ambient;

    output.uv = vin.uv;
    
    

    return output;
}