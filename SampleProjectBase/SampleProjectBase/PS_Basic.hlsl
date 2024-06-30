Texture2D myTexture : register(t0); // テクスチャー
SamplerState mySampler : register(s0); // サンプラー

//// マテリアル
//struct Material
//{
//    float4 diffuse; // 色
//    float4 ambient; // 環境光
//    float4 specular; // 鏡面反射
//    float4 emissive; // 自発光
//    float shininess; // 光沢
//    bool isTextureEnable; // テクスチャを使用しているかフラグ
//};

//// マテリアル
//cbuffer Material : register(b1)
//{
//    Material material;
//}

struct PS_INPUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
   
    // テクスチャを使用しているなら
    color = myTexture.Sample(mySampler, pin.uv);
    color *= pin.color;
    
    return color;
}