// Slot0 座標変換
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

cbuffer LightWVP : register(b1)
{
    float4x4 Lworld;
    float4x4 Lview;
    float4x4 LProj;
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
    // ライトから見た座標
    float4 lightSpacePos : POSITION0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;

    // 各行列を計算する
    float4 pos = float4(vin.pos, 1.0f);
     // ワールド座標
    pos = mul(pos, world);
    float4 worldPos = pos;
     // スクリーン座標
    pos = mul(pos, view);
    // プロジェクション座標
    pos = mul(pos, projection);
    
    // ライト空間座標
    output.lightSpacePos = mul(worldPos, Lview);
    output.lightSpacePos = mul(output.lightSpacePos, LProj);
    
    output.pos = pos;
    output.color = vin.color;
    output.uv = vin.uv;

    return output;
}

