struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : SV_Position0;
};

struct PointLight
{
    // 座標
    float3 pos;
    // 色
    float4 color;
    // 範囲
    float range;
};

cbuffer LightBuf : register(b0)
{
    PointLight lights[30];
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    for (int loop = 0; loop < 30; loop++)
    {
        // ライトへのベクトルを求める
        float3 toLightVec = lights[loop].pos - pin.worldPos.xyz;
        float3 V = normalize(toLightVec); // 正規化ベクトル
        float toLightLen = length(toLightVec); // ベクトルの長さを求める
		
		// 光のあたり加減を求める
        float3 N = normalize(pin.normal); // 単位法線ベクトル
        float dotNV = saturate(dot(N, V)); // ライトへのベクトルと法線ベクトルの内積を求める
		
		// 距離に応じて光の減衰を求める
		// 距離が近ければ明るく、離れていれば暗くなる
        float attenuation = saturate(1.0f - toLightLen / lights[loop].range);
		// 距離に応じて光の当たる面が乗算で増えていく
        attenuation = pow(attenuation, 2.0f);
		
		// ピクセルの色を求める
        color.rgb += dotNV * lights[loop].color.rgb * attenuation;
    }

    return color;

}