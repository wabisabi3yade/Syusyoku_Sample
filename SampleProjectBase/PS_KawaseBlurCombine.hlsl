struct PS_IN
{
   // スクリーン座標
    float4 pos : SV_POSITION0;
    
    // uv座標
    float2 uv : TEXCOORD0;
};

// 川瀬式ブラーの各ブラーテクスチャ
Texture2D blurTex0 : register(t0);
Texture2D blurTex1 : register(t1);
Texture2D blurTex2 : register(t2);
Texture2D blurTex3 : register(t3);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    // ボケ画像をサンプリングして、平均を取る
    float4 combineColor = blurTex0.Sample(samp, pin.uv);
    combineColor += blurTex1.Sample(samp, pin.uv);
    combineColor += blurTex2.Sample(samp, pin.uv);
    combineColor += blurTex3.Sample(samp, pin.uv);
    combineColor /= 4.0f;
    combineColor.a = 1.0f;
    
	return combineColor;
}