// Slot0 座標変換
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

// Slot1 ライトのビュー変換
cbuffer LightWVP : register(b1)
{
    matrix Lworld;
    matrix Lview;
    matrix Lprojection;
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
    // ライトビュー上での座標
    float4 lightSpacePos : POSITION1;
};

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;

    // 各行列を計算する
    float4 pos = float4(vin.pos, 1.0f);
    // ワールド座標
    pos = mul(pos, world);
    output.worldPos = pos;
    
     // スクリーン座標
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    // ライトビュー上での
    output.lightSpacePos = mul(output.worldPos, Lview);
    output.lightSpacePos = mul(output.lightSpacePos, Lprojection);
    
    output.pos = pos;
    output.color = vin.color;
    output.uv = vin.uv;
    
    output.normal = normalize(mul(vin.normal, (float3x3) world));
    return output;
}