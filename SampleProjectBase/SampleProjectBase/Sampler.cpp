#include "pch.h"
#include "Sampler.h"

Sampler::Sampler()
{

}

Sampler::~Sampler()
{
    SAFE_RELEASE(pSampler);
}

bool Sampler::Init(ID3D11Device& _device)
{
    // サンプラー作成
    D3D11_SAMPLER_DESC smpDesc;

    ::ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
    smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    HRESULT hr = _device.CreateSamplerState(&smpDesc, &pSampler);
    if (FAILED(hr))
    {
        ImGuiDebugLog::AddDebugLog("サンプラー初期化失敗");
        return false;
    }

    return true;
       
}

