struct VS_IN
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
};

// Slot0 座標変換
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

VS_OUTPUT main(VS_IN vin)
{
    VS_OUTPUT output;
    output.pos = float4(vin.pos, 1.0f);
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    // スケール成分を除く
    float3x3 rotationMatrix = (float3x3) world;
    
    output.normal = mul(vin.normal, rotationMatrix);
    output.normal = normalize(output.normal);
    output.color = vin.color;
    output.uv = vin.uv;
    
    return output;
}