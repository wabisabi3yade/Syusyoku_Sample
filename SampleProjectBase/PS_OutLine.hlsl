#define SCREEN_WIDTH (1280.0f) // ウィンドウ横
#define SCREEN_HEIGHT (720.0f) // ウィンドウ縦

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // 頂点色
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

cbuffer Camera : register(b0)
{
    float4 cameraPos;
}

cbuffer OutLine : register(b1)
{
    float ditherDistancePower; // 距離によるディザーにで透明にする係数(大きいほど近づいたときに消える)
    float ditherDistanceOffset; // ディザーのオフセット(5を設定すると5の距離まではすべて消える)
    float2 dummy;
}

Texture2D ditherTex : register(t0); // アルファディザーテクスチャー
SamplerState mySampler : register(s0); // サンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    // アルファディザー
    float len = length(pin.worldPos.xyz - cameraPos.xyz);
    float alpha = color.a * saturate(len * ditherDistancePower - ditherDistanceOffset);
    
    // スクリーンの座標に応じて、
    // ディザパターンを適用する位置を計算
    float2 ditherUV = pin.pos.xy / float2(SCREEN_WIDTH, SCREEN_HEIGHT);
    ditherUV = frac(ditherUV * 200.0f);
    
    // ディザパターンの取得
    float dither = ditherTex.Sample(mySampler, ditherUV).r;
    if (alpha - 1e-6 < dither)
    {
        // 透明度0で見えないけどそこにある、ではなく完全に表示を行わない
        discard;
    }
    
    return color;
}