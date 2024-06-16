// Slot0 座標変換
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

cbuffer UV_OFFSET : register(b1)
{
    float2 uv_offset;
};


struct VS_INPUT
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 color : COLOR; // 頂点色
    float2 uv : TEXCOORD0; // uv座標
    float3 normal : NORMAL0; // 法線ベクトル
};

// ピクセルシェーダーに渡す
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;

    // 各行列を計算する
    float4 pos = float4(vin.pos, 1.0);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;


    output.color = vin.color;
    output.uv = vin.uv + uv_offset;

    return output;
}