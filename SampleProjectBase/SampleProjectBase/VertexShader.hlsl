// Slot0 座標変換
cbuffer cbTransform : register(b0) {
    matrix Transform;
};

// Slot1 ビュー変換
cbuffer cbProjection : register(b1) { 
    matrix View;
};

// Slot2 投影変換
cbuffer cbProjection : register(b2) { 
    matrix Projection;
};

struct VS_INPUT
{
    float3 Pos : POSITION; // 頂点座標（モデル座標系）
    float4 Col : COLOR; // 頂点色
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Col : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 各行列を計算する
    float4 pos = float4(input.Pos, 1.0);
    pos = mul(pos, Transform);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.Pos = pos;

    output.Col = input.Col;

    return output;
}