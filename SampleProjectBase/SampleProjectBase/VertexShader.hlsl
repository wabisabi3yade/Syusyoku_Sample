// Slot0 座標変換
cbuffer cbTransform : register(b0) {
    matrix transform;
};

// Slot1 ビュー変換
cbuffer cbView : register(b1) { 
    matrix view;
};

// Slot2 投影変換
cbuffer cbProjection : register(b2) { 
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 col : COLOR; // 頂点色
    float2 uv : TEXCOORD0;   // uv座標
    float3 normal : NORMAL0;    // 法線ベクトル
};

// ピクセルシェーダーに渡す
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 各行列を計算する
    float4 pos = float4(input.pos, 1.0);
    pos = mul(pos, transform);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;


    output.col = input.col;
    output.uv = input.uv;

    return output;
}