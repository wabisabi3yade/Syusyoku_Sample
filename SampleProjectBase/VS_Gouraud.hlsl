
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
    float4 color; // 色
    float3 position; // 座標  
    float dummy;
};

// ディレクションライト
struct DirectionLight
{
    LightBase base;
    float4 ambiemtColor; // 環境色
    float3 direction; // 方向
    float dummy;
};

//// ポイントライト
//struct PointLight
//{
//    LightBase base;
//    float range; // 範囲
//    float3 dummy;
//};

//// スポットライト
//struct SpotLight
//{
//    LightBase base;
//    float3 direction; // 方向
//    float distance; // 距離
//    float angleRange; // 角度範囲
//    float3 dummy;
//};

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

// ライト
cbuffer BufLight : register(b2)
{
    DirectionLight dirL;
    //PointLight pointL;
    //SpotLight spotL;
};

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
    // ワールド座標
    float4 worldPos : POSITION0;
};

// DirectionLightが与える色の影響を計算する
float4 CalcDirLight(float3 _normal)
{
    float4 color = float4(0, 0, 0, 0);
    float3 arcLightDir = -dirL.direction;
    
    // 法線ベクトルと逆ライトベクトルの内積
    float dotNL = dot(_normal, arcLightDir);
    
    color = saturate(dotNL * dirL.base.color + dirL.ambiemtColor);
    
    return color;
}

VS_OUTPUT main(VS_IN vin)
{
    VS_OUTPUT output;
    output.pos = float4(vin.pos, 1.0f);
    output.pos = mul(output.pos, world);
    output.worldPos = output.pos;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

	// 頂点シェーダーで陰の計算
    float3 N = vin.normal;
    
    float3x3 rotationMatrix = (float3x3) world;
    rotationMatrix[0] = normalize(rotationMatrix[0]);
    rotationMatrix[1] = normalize(rotationMatrix[1]);
    rotationMatrix[2] = normalize(rotationMatrix[2]);
    
    N = mul(N, rotationMatrix);
    output.normal = normalize(N);
    
	// マテリアル色
    output.color = material.diffuse;
    // ディレクションライトに色乗算
    output.color.rgb *= CalcDirLight(N).rgb;
   
    output.uv = vin.uv;
    
    return output;
}