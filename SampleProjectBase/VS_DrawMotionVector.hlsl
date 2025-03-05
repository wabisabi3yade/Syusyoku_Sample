struct VS_IN
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 color : COLOR0; // 頂点色
    float2 uv : TEXCOORD0; // uv座標
};

struct VS_OUT
{
    float4 pos : SV_POSITION0; // 頂点座標（モデル座標系）
    float2 uv : TEXCOORD0; // uv座標
    float2 motionVector : TEXCOORD1; // 前フレームからの速度ベクトル
};

// WVP行列
struct WVP
{
    matrix world;
    matrix view;
    matrix projection;
};

// モーションブラー
cbuffer MotionBlurParam : register(b0)
{
    // 前回のWVP
    WVP prevWVP;
    
    // 今回のWVP
    WVP curWVP;
};


VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    float4 pos = float4(vin.pos, 1.0f);
    
    // 現在の頂点位置を取得
    float4 curPos = mul(pos, curWVP.world);
    curPos = mul(curPos, curWVP.view);
    curPos = mul(curPos, curWVP.projection);
    vout.pos = curPos;
   
    // 1フレーム前の頂点位置を取得
    float4 prevPos = mul(pos, prevWVP.world);
    prevPos = mul(prevPos, prevWVP.view);
    prevPos = mul(prevPos, prevWVP.projection);
    
    // uv
    vout.uv = vin.uv;
    
    // 差分(速度)を取得する
    vout.motionVector = curPos.xy / curPos.w - prevPos.xy / prevPos.w;
    vout.motionVector *= 0.5f;
    
	return vout;
}