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
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer BoneMatrixBuffer : register(b1)
{
    matrix BoneMatrix[100];
}

#define OUTLINE_SCALE (0.01f)

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    matrix Comb = BoneMatrix[vin.boneIndex[0]] * vin.boneWeight[0];
    Comb += BoneMatrix[vin.boneIndex[1]] * vin.boneWeight[1];
    Comb += BoneMatrix[vin.boneIndex[2]] * vin.boneWeight[2];
    Comb += BoneMatrix[vin.boneIndex[3]] * vin.boneWeight[3];
    
    // 伸ばす
    vout.pos.xyz += normalize(vin.normal) * OUTLINE_SCALE;
    
    float4 Pos = float4(vin.pos, 1.0f);
    Pos = mul(Pos, Comb);
    vout.pos = Pos;
	
	// 法線ベクトルを補正
    float4 Normal = float4(vin.normal.xyz, 0.0f);
 
    Comb._41 = 0.0f; // 移動成分を消す
    Comb._42 = 0.0f;
    Comb._43 = 0.0f;
    Comb._44 = 1.0f;

    Normal = mul(Normal, Comb);
    vout.normal = Normal;
	
    vout.pos = mul(vout.pos, world);

    // ピクセルシェーダーでワールド座標を使用するので
    vout.worldPos = vout.pos;

    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    // スケール成分を除く
    float3x3 rotationMatrix = (float3x3) world;
    rotationMatrix[0] = normalize(rotationMatrix[0]);
    rotationMatrix[1] = normalize(rotationMatrix[1]);
    rotationMatrix[2] = normalize(rotationMatrix[2]);
    
    vout.normal = mul(vout.normal, rotationMatrix);
    vout.normal = normalize(vout.normal);
    
    return vout;
}