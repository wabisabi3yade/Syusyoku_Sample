// Slot0 投影行列
cbuffer Projection : register(b0)
{
    // 投影行列
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 color : COLOR0; // 頂点色
    float2 uv : TEXCOORD0; // uv座標
};

// ピクセルシェーダーに渡す
struct VS_OUTPUT
{
    // スクリーン座標
    float4 pos : SV_POSITION0;
     // 頂点カラー
    float4 color : COLOR0;
    // uv座標
    float2 uv : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;
    
    output.pos = float4(vin.pos.xyz, 1.0f);
    output.pos = mul(output.pos, projection);
    
    output.color = vin.color;
    output.uv = vin.uv;
    
	return output;
}