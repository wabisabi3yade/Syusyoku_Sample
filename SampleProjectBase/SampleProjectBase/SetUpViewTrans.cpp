#include "pch.h"
#include "SetUpViewTrans.h"

#include "RenderParam.h"
#include "IGetRenderResource.h"

bool SetUpViewTrans::SetUpViewTransform(const DirectX::XMMATRIX& _viewMatrix, 
    IGetRenderResource* _pRenderResource)
{
    auto& cb = _pRenderResource->GetParameter().cbViewSet;
    DirectX::XMStoreFloat4x4(&cb.data.view, DirectX::XMMatrixTranspose(_viewMatrix));

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto pDeviceContext = _pRenderResource->GetDeviceContext();
    // CBuffer�ɂЂ��Â��n�[�h�E�F�A���\�[�X�}�b�v�擾�i���b�N���Ď擾�j
    HRESULT hr = pDeviceContext->Map(
        cb.pBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource);
    if (FAILED(hr)) {
        ImGuiDebugLog::AddDebugLog("�r���[�ϊ��s�񏀔��ŃG���[");
        return false;
    }
    CopyMemory(mappedResource.pData, &cb.data, sizeof(cb.data));
    // �}�b�v����
    pDeviceContext->Unmap(cb.pBuffer, 0);

    // VS��ViewMatrix���Z�b�g
    pDeviceContext->VSSetConstantBuffers(1, 1, &cb.pBuffer);

    return true;
}
