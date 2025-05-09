struct PS_IN
{
    float4 pos : POSITION; // 頂点座標（モデル座標系）
    float2 uv : TEXCOORD0; // uv座標
    float2 motionVector : TEXCOORD1; // 前フレームからの速度ベクトル
};

float4 main(PS_IN pin) : SV_TARGET
{
    // モーションベクトルを書き出す
    return float4(pin.motionVector, 0.0f, 1.0f);
}