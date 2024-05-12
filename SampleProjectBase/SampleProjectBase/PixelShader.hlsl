Texture2D myTexture : register(t0);	// テクスチャー
SamplerState mySampler : register(s0);	// サンプラー

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャからデータを取得
	color = myTexture.Sample(mySampler, input.uv);

	return color;
}