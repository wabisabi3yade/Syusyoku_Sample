#define SCREEN_WIDTH (1280.0f) // �E�B���h�E��
#define SCREEN_HEIGHT (720.0f) // �E�B���h�E�c

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // ���_�F
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

cbuffer Camera : register(b0)
{
    float4 cameraPos;
}

cbuffer OutLine : register(b1)
{
    float ditherDistancePower; // �����ɂ��f�B�U�[�ɂœ����ɂ���W��(�傫���قǋ߂Â����Ƃ��ɏ�����)
    float ditherDistanceOffset; // �f�B�U�[�̃I�t�Z�b�g(5��ݒ肷���5�̋����܂ł͂��ׂď�����)
    float2 dummy;
}

Texture2D ditherTex : register(t0); // �A���t�@�f�B�U�[�e�N�X�`���[
SamplerState mySampler : register(s0); // �T���v���[

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = pin.color;
    
    // �A���t�@�f�B�U�[
    float len = length(pin.worldPos.xyz - cameraPos.xyz);
    float alpha = color.a * saturate(len * ditherDistancePower - ditherDistanceOffset);
    
    // �X�N���[���̍��W�ɉ����āA
    // �f�B�U�p�^�[����K�p����ʒu���v�Z
    float2 ditherUV = pin.pos.xy / float2(SCREEN_WIDTH, SCREEN_HEIGHT);
    ditherUV = frac(ditherUV * 200.0f);
    
    // �f�B�U�p�^�[���̎擾
    float dither = ditherTex.Sample(mySampler, ditherUV).r;
    if (alpha - 1e-6 < dither)
    {
        // �����x0�Ō����Ȃ����ǂ����ɂ���A�ł͂Ȃ����S�ɕ\�����s��Ȃ�
        discard;
    }
    
    return color;
}