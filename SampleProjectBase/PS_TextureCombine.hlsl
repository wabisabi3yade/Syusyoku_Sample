// レンダーターゲットを重ねたりするピクセルシェーダー
struct PS_IN
{
   // スクリーン座標
    float4 pos : SV_POSITION0;
    
    // uv座標
    float2 uv : TEXCOORD0;
};

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);

SamplerState mySampler : register(s0); // サンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = texture0.Sample(mySampler, pin.uv);
    color += texture1.Sample(mySampler, pin.uv);
    color.a = 1.0f;
    
    return color;
}