// Slot0 座標変換
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct Material
{
    float4 diffuse; // 色
    float4 ambient; // 環境光
    float4 specular; // 鏡面反射
    float4 emissive; // 自発光
    float shininess; // 光沢
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
    VS_OUTPUT output;

    // 各行列を計算する
    float4 pos = float4(vin.pos, 1.0);
     // ワールド座標
    pos = mul(pos, world);
    output.worldPos = pos;
    
     // スクリーン座標
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;
    output.uv = vin.uv;
    
    float3x3 rotationMatrix = (float3x3)world;
    rotationMatrix[0] = normalize(rotationMatrix[0]);
    rotationMatrix[1] = normalize(rotationMatrix[1]);
    rotationMatrix[2] = normalize(rotationMatrix[2]);
    
    output.normal = mul(vin.normal, rotationMatrix);
    output.normal = normalize(output.normal);

    return output;
}