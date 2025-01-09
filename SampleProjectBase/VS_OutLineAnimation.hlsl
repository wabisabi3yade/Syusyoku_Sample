
struct VS_IN
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float4 color : COLOR0; // ���_�F
    float2 uv : TEXCOORD0; // uv���W
    float3 normal : NORMAL0; // �@���x�N�g��
    int4 boneIndex : BONEINDEX; // �{�[���C���f�b�N�X
    float4 boneWeight : BONEWEIGHT; // �E�F�C�g
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // ���_�F
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
}

cbuffer OutLine : register(b1)
{
    float4 color;
    float lineScale;
    float3 dummy;
}

cbuffer BoneMatrixBuffer : register(b2)
{
    matrix BoneMatrix[100];
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    matrix Comb = BoneMatrix[vin.boneIndex[0]] * vin.boneWeight[0];
    Comb += BoneMatrix[vin.boneIndex[1]] * vin.boneWeight[1];
    Comb += BoneMatrix[vin.boneIndex[2]] * vin.boneWeight[2];
    Comb += BoneMatrix[vin.boneIndex[3]] * vin.boneWeight[3];
    
    float4 Pos = float4(vin.pos, 1.0f);
    Pos = mul(Pos, Comb);
    vout.pos = Pos;
	
	// �@���x�N�g����␳
    float4 Normal;
    Normal = float4(vin.normal.xyz, 0.0f);
 
    Comb._41 = 0.0f; // �ړ�����������
    Comb._42 = 0.0f;
    Comb._43 = 0.0f;
    Comb._44 = 1.0f;

    Normal = mul(Normal, Comb);
    vout.normal = Normal;
    
    vout.pos = mul(vout.pos, world);
    vout.normal = normalize(mul(vout.normal, (float3x3) world));
    
    vout.pos.xyz += vout.normal * lineScale;
    vout.worldPos = vout.pos;
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    vout.uv = vin.uv;
    vout.color = color;
    return vout;
}