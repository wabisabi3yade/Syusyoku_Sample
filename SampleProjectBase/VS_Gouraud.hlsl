
struct Material
{
    float4 diffuse; // 色
    float4 ambient; // 環境光
    float4 specular; // 鏡面反射
    float4 emissive; // 自発光
    float shininess; // 光沢
    int isTextureEnable; // テクスチャフラグ
    float2 dummy;
};

// ライトの基礎パラメータ
struct LightBase
{
    float4 color;   // 色
    float3 position;    // 座標  
    float dummy;
};

// ディレクションライト
struct DirectionLight
{
    LightBase base;
    float4 ambiemtColor; // 環境色
    float3 direction;   // 方向
    float dummy;
};

// ポイントライト
struct PointLight
{
    LightBase base;
    float range;    // 範囲
    float3 dummy;
};

// スポットライト
struct SpotLight
{
    LightBase base;
    float3 direction;   // 方向
    float distance; // 距離
    float angleRange;   // 角度範囲
    float3 dummy;
};

// Slot0 座標変換
cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};


// Slot1 マテリアル
cbuffer MaterialBuffer : register(b1)
{
    Material material;
};

// ディレクションライト
cbuffer BufDirectionL : register(b2)
{
    DirectionLight directionL;
    
};

struct VS_INPUT
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float2 uv : TEXCOORD0; // uv座標
    float4 color : COLOR0; // 頂点色
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

float3 

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT output;
    output.pos = float4(vin.pos, 1.0f);
    output.pos = mul(output.pos, world);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

	// 頂点シェーダーで陰の計算
    float3 N = vin.normal;
	// 計算前に手を加える
    N = mul(N, (float3x3) world);
    output.normal = N;
    
	// マテリアル色
    output.color = material.diffuse;
    // ディレクションライトに色乗算
    output.color.rgb *= directionL.base.color.rgb;
   
    output.uv = vin.uv;
    
    return output;
}