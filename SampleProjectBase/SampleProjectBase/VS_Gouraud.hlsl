
struct Material
{
    float4 diffuse; // 色
    float4 ambient; // 環境光
    float4 specular; // 鏡面反射
    float4 emissive; // 自発光
    float shininess; // 光沢
    bool isTexEnable;
};

// ライトの基礎パラメータ
struct LightBase
{
    float3 position;
    float4 color;
};

// ディレクションライト
struct DirectionLight
{
    LightBase base;
    float3 direction;
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


cbuffer BufDirectionL
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

VS_OUTPUT main(VS_INPUT vin)
{
    VS_OUTPUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.worldPos = vout.pos;
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, projection);

	// 頂点シェーダーで陰の計算
    float3 N = vin.normal;
    float3 L = directionL.direction.xyz;
	// 計算前に手を加える
    N = mul(N, (float3x3) world);
    L = -L;
	// 正規化
    N = normalize(N);
    L = normalize(L);
	// 陰計算
    float diffuse = saturate(dot(N, L));
	// 色決定
    vout.color = float4(diffuse + directionL.base.color.rgb, 1.0f);

    vout.uv = vin.uv;

    return vout;
}