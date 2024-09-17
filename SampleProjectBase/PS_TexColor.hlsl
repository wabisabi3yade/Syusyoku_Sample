Texture2D myTexture : register(t0); // �e�N�X�`���[
SamplerState mySampler : register(s0); // �T���v���[

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

// �}�e���A��
struct TexEnable
{
    int isTextureEnable; // �e�N�X�`���g�p
    float3 dummy;
};


cbuffer BufTexEnable : register(b0)
{
    TexEnable texEnable;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    // �e�N�X�`�����g�p����Ă���Ȃ�
    if (texEnable.isTextureEnable != 0)
    {
        color *= myTexture.Sample(mySampler, pin.uv);
    }
    
    return color;
}