struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : SV_Position0;
};

struct PointLight
{
    // ���W
    float3 pos;
    // �F
    float4 color;
    // �͈�
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
        // ���C�g�ւ̃x�N�g�������߂�
        float3 toLightVec = lights[loop].pos - pin.worldPos.xyz;
        float3 V = normalize(toLightVec); // ���K���x�N�g��
        float toLightLen = length(toLightVec); // �x�N�g���̒��������߂�
		
		// ���̂�������������߂�
        float3 N = normalize(pin.normal); // �P�ʖ@���x�N�g��
        float dotNV = saturate(dot(N, V)); // ���C�g�ւ̃x�N�g���Ɩ@���x�N�g���̓��ς����߂�
		
		// �����ɉ����Č��̌��������߂�
		// �������߂���Ζ��邭�A����Ă���ΈÂ��Ȃ�
        float attenuation = saturate(1.0f - toLightLen / lights[loop].range);
		// �����ɉ����Č��̓�����ʂ���Z�ő����Ă���
        attenuation = pow(attenuation, 2.0f);
		
		// �s�N�Z���̐F�����߂�
        color.rgb += dotNV * lights[loop].color.rgb * attenuation;
    }

    return color;

}