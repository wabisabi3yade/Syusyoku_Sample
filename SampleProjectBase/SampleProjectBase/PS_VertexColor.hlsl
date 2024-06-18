// ピクセルシェーダーに渡す
struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

// 頂点色を描画する
float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
	return color;
}