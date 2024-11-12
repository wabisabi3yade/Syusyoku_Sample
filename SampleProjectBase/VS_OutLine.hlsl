
struct VS_IN
{
    float3 pos : POSITION; // ���_���W�i���f�����W�n�j
    float4 color : COLOR0; // ���_�F
    float2 uv : TEXCOORD0; // uv���W
    float3 normal : NORMAL0; // �@���x�N�g��
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
}

#define OUTLINE_SCALE (0.5f)

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    // �X�P�[������������
    //float3x3 rotationMatrix = (float3x3) world;
    //rotationMatrix[0] = normalize(rotationMatrix[0]);
    //rotationMatrix[1] = normalize(rotationMatrix[1]);
    //rotationMatrix[2] = normalize(rotationMatrix[2]);
    
    //vout.normal = mul(vin.normal, rotationMatrix);
    //vout.normal = normalize(vout.normal);
    
    vout.pos = float4(vin.pos, 1.0f);
    
    vout.pos.xyz += normalize(vin.normal) * OUTLINE_SCALE;
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    vout.uv = vin.uv;
    return vout;
}