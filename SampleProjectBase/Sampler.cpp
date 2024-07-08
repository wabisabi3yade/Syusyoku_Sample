#include "pch.h"
#include "Sampler.h"
bool Sampler::Init(ID3D11Device& _device)
{
    // �T���v���[�쐬
    D3D11_SAMPLER_DESC smpDesc;

    ::ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
    smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    ID3D11SamplerState* samplePtr = pSampler.get();
    HRESULT hr = _device.CreateSamplerState(&smpDesc, &samplePtr);
    if (FAILED(hr))
    {
        ImGuiDebugLog::Add("�T���v���[���������s");
        return false;
    }

    return true;
       
}

