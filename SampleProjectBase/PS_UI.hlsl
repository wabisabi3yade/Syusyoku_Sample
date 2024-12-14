Texture2D myTexture : register(t0); // �e�N�X�`���[
SamplerState mySampler : register(s0); // �T���v���[

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

// �e�N�X�`���g�p
struct TexParam
{
    int isTextureEnable; // �e�N�X�`���g�p
    float alpha;    // ���l
    float2 dummy;
};


cbuffer BufTexParam : register(b0)
{
    TexParam texParam;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    color.a = texParam.alpha;
    
    // �e�N�X�`�����g�p����Ă���Ȃ�
    if (texParam.isTextureEnable != 0)
    {
        color *= myTexture.Sample(mySampler, pin.uv);
    }
    
  
    
    return color;
}