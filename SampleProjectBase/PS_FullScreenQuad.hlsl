struct PS_IN
{
   // スクリーン座標
    float4 pos : SV_POSITION0;
    
    // uv座標
    float2 uv : TEXCOORD0;
};

Texture2D sceneTex : register(t0);
Texture2D blurTex : register(t1);

SamplerState mySampler : register(s0); // サンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float2 motionVector = blurTex.Sample(mySampler, pin.uv).rg;
    
    color = sceneTex.Sample(mySampler, pin.uv);
    
    // // モーションベクトルに沿ってブラーを適用
    //for (int i = 1; i < 10; ++i)
    //{
    //    float t = (float) i / 10.0;
    //    float2 offset = pin.uv + motionVector * t;
    //    color += sceneTex.Sample(mySampler, offset);
    //}
    
    return color;
}