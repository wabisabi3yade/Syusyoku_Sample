// モーションブラー

struct PS_IN
{
   // スクリーン座標
    float4 pos : SV_POSITION0;
    
    // uv座標
    float2 uv : TEXCOORD0;
};

Texture2D sceneTex : register(t0);	// シーン描画
Texture2D motionVectorTex : register(t1); // モーションベクトルテクスチャ
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    // 色を取得
    float4 color = sceneTex.Sample(samp, pin.uv);
    float2 motionVector = motionVectorTex.Sample(samp, pin.uv).rg;
    
    float blurAmount = length(motionVector) * 10; // ブラーの強さ（調整可能）
    
    // モーションベクトルの方向に沿って複数回サンプリング
    [unroll]
    for (int i = 1; i <= 10; i++)
    {
        float weight = i / 10.0f;
        float2 offset = motionVector * weight / blurAmount;
        color += sceneTex.Sample(samp, pin.uv + offset);
    }
    
    // 平均する
    color /= 10.0f;
    color.a = 1.0f;
    
    return color;
}