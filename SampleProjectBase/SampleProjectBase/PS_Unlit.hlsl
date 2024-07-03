Texture2D myTexture : register(t0); // テクスチャー
SamplerState mySampler : register(s0); // サンプラー

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

// マテリアル
struct Material
{
    float4 diffuse; // 色
    float4 ambient; // 環境光
    float4 specular; // 鏡面反射
    float4 emissive; // 自発光
    float shininess; // 光沢
};

// ライトの基礎パラメータ
struct LightBase
{
    float3 position;
    float4 color;
};

// ポイントライト
struct PointLight
{
    LightBase base;
    float range;
};


// スポットライト
struct SpotLight
{
    LightBase base;
    float distance;
    float rangeAngle;
    float3 direction;
};

cbuffer BufMaterial : register(b0)
{
    Material material;
}

cbuffer BufPoint : register(b1)
{
    PointLight pointLights[30];
}

cbuffer BufSpot : register(b2)
{
    SpotLight spotLights[30];
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    
	return color;
}