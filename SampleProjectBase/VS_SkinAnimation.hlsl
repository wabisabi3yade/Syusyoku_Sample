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
    matrix BoneMatrix[400];
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
	
	// ワンスキン頂点ブレンドの処理
    float4x4 Comb = (float4x4) 0;
    Comb._11 = 1.0f;
    Comb._22 = 1.0f;
    Comb._33 = 1.0f;
    Comb._44 = 1.0f;
    
    for (int i = 0; i < 4; i++)
    {
		// 重みを計算しながら行列生成
        Comb += BoneMatrix[vin.boneIndex[i]] * vin.boneWeight[i];
    }
	
    float4 Pos;
    Pos = float4(vin.pos, 1.0f);
    Pos = mul(Comb, Pos);
    vout.pos = Pos;
	
	// 法線ベクトルを補正
    float4 Normal;
    Normal = float4(vin.normal.xyz, 0.0);
 
    Comb._41 = 0.0f; // 移動成分を消す
    Comb._42 = 0.0f;
    Comb._43 = 0.0f;
    Comb._44 = 1.0f;

    Normal = mul(Comb, float4(vin.normal, 0.0));
    normalize(Normal);
    vin.normal = Normal;
	
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);

    // ピクセルシェーダーでワールド座標を使用するので
    vout.worldPos = vout.pos;

    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);

    vout.uv = vin.uv;
    vout.normal = mul(vin.normal, (float3x3) world);
    vout.color = vin.color;
    
    return vout;
}