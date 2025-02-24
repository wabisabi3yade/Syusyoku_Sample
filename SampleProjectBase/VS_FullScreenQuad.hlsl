struct VS_IN
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 color : COLOR0; // 頂点色
    float2 uv : TEXCOORD0; // uv座標
};

struct VS_OUT
{
    float4 pos: SV_POSITION0; // 頂点座標（モデル座標系）
    float2 uv : TEXCOORD0; // uv座標
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.uv = vin.uv;
    
    return vout;
}