Texture2D myTexture : register(t0);	// テクスチャー
SamplerState mySampler : register(s0);	// サンプラー

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT pin) : SV_TARGET
{ 
	// テクスチャからデータを取得
    float4 color = myTexture.Sample(mySampler, pin.uv);

	return color;
}