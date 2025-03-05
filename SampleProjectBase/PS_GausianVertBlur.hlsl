struct PS_IN
{
    float4 pos : SV_POSITION0; // 頂点座標（モデル座標系）
    float2 uv : TEXCOORD0; // uv座標
};

// ウィンドウサイズ
cbuffer WindowSize : register(b0)
{
    int windowWidth;
    int windowHeight;
    int2 dummy2;
}

Texture2D inputTexture : register(t0);
SamplerState samp : register(s0);

// 横方向ブラー
float4 main(PS_IN pin) : SV_TARGET
{
    
    float2 texelSize = float2(0, 1.0 / windowHeight); // 縦方向のみ
    float weights[5] = { 0.227, 0.194, 0.121, 0.054, 0.016 };

    float3 blur = inputTexture.Sample(samp, pin.uv).rgb * weights[0];

    [unroll]
    for (int i = 1; i < 5; i++)
    {
        blur += inputTexture.Sample(samp, pin.uv + float2(0, texelSize.y * i)).rgb * weights[i];
        blur += inputTexture.Sample(samp, pin.uv - float2(0, texelSize.y * i)).rgb * weights[i];
    }

    return float4(blur, 1.0);
}