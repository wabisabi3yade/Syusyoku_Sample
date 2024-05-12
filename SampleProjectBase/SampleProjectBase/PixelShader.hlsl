Texture2D myTexture : register(t0);	// �e�N�X�`���[
SamplerState mySampler : register(s0);	// �T���v���[

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`������f�[�^���擾
	color = myTexture.Sample(mySampler, input.uv);

	return color;
}