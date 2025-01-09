
struct VS_IN
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 color : COLOR0; // 頂点色
    float2 uv : TEXCOORD0; // uv座標
    float3 normal : NORMAL0; // 法線ベクトル
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // 頂点色
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

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.normal = normalize(mul(vin.normal, (float3x3) world));
    
    vout.pos.xyz += vout.normal * lineScale;
    vout.worldPos = vout.pos;
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    vout.uv = vin.uv;
    vout.color = color;
    return vout;
}