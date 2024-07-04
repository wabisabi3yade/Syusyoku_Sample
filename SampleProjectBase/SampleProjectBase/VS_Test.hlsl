// Slot0 座標変換
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 color : COLOR0; // 頂点色
    float2 uv : TEXCOORD0; // uv座標
    float3 normal : NORMAL0; // 法線ベクトル
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
    // 法線ベクトル
    float3 normal : NORMAL0;
    // ワールド座標
    float4 worldPos : POSITION0;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT vout;
    
     // 各行列を計算する
    float4 pos = float4(vin.pos, 1.0);
     // ワールド座標
    pos = mul(pos, world);
    vout.worldPos = pos;
    
     // スクリーン座標
    pos = mul(pos, view);
    pos = mul(pos, projection);
    vout.pos = pos;
    vout.uv = vin.uv;
    vout.normal = mul(vin.normal, (float3x3) world);
    
    vout.color = vin.color;

    return vout;
}