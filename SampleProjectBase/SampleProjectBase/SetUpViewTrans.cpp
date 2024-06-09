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
    // CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
    HRESULT hr = pDeviceContext->Map(
        cb.pBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource);
    if (FAILED(hr)) {
        ImGuiDebugLog::AddDebugLog("ビュー変換行列準備でエラー");
        return false;
    }
    CopyMemory(mappedResource.pData, &cb.data, sizeof(cb.data));
    // マップ解除
    pDeviceContext->Unmap(cb.pBuffer, 0);

    // VSにViewMatrixをセット
    pDeviceContext->VSSetConstantBuffers(1, 1, &cb.pBuffer);

    return true;
}
