struct PS_IN
{
   // スクリーン座標
    float4 pos : SV_POSITION0;
    
    // uv座標
    float2 uv : TEXCOORD0;
};

SamplerState samp : register(s0);
Texture2D tex : register(t0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
	
    // サンプリングしたカラーの明るさを計算
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    
    // clip()関数でマイナスはスキップ
    float threshold = 0.85f;
    clip(t - threshold);
    return color;
}