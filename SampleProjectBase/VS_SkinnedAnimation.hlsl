#define MAX_BONE 400

struct VS_IN
{
    float3 pos : POSITION; // 頂点座標（モデル座標系）
    float4 color : COLOR0; // 頂点色
    float2 uv : TEXCOORD0; // uv座標
    float3 normal : NORMAL0; // 法線ベクトル
    int4 boneIndex : BONEINDEX; // ボーンインデックス
    float4 boneWeight : BONEWEIGHT; // ウェイト
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};


cbuffer WVP : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};


cbuffer BoneMatrixBuf : register(b1)
{
    float4x4 BoneMatricies[MAX_BONE];
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    vout.pos = float4(0, 0, 0, 0);
    vout.color = float4(0, 0, 0, 0);
    vout.normal = float3(0, 0, 0);
    vout.uv = float2(0, 0);
    vout.worldPos = float4(0, 0, 0, 0);
    
    //float4x4 skinMatrix = (float4x4) 0;
    
    //// ボーン行列の加重平均を計算
    ////skinMatrix += BoneMatricies[vin.boneIndex[0]] * vin.boneWeight[0];
    ////skinMatrix += BoneMatricies[vin.boneIndex[1]] * vin.boneWeight[1];
    ////skinMatrix += BoneMatricies[vin.boneIndex[2]] * vin.boneWeight[2];
    ////skinMatrix += BoneMatricies[vin.boneIndex[3]] * vin.boneWeight[3];
    
    ////// ボーン行列で移動
    ////vout.pos = mul(float4(vin.pos, 1.0f), skinMatrix);
    //vout.normal = vin.normal;
    ////vout.normal = mul(float4(vin.normal, 1.0f), skinMatrix);
    
    //// wvp反映
    //vout.pos = mul(float4(vin.pos, 1.0f), world);
    //vout.worldPos = vout.pos;
    //vout.pos = mul(vout.pos, view);
    //vout.pos = mul(vout.pos, projection);
    
    //vout.normal = mul(float4(vout.normal, 1.0f), world);
    
    //// 色・UV
    //vout.color = vin.color;
    //vout.uv = vin.uv;
   
    return vout;
}