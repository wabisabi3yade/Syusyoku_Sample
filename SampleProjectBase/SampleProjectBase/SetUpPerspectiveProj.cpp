#include "pch.h"
#include "SetUpPerspectiveProj.h"

#include "RenderParam.h"

// �f�t�H���g�l
constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

SetUpPerspectiveProj::SetUpPerspectiveProj()
{
    fov = DEFAULT_FOV;	// ����p
    nearZ = DEFAULT_NEARZ;	// �`�拗���i�ŒZ�j
    farZ = DEFAULT_FARZ;	// �`�拗��(�Œ�)
}

SetUpPerspectiveProj::~SetUpPerspectiveProj()
{
}

bool SetUpPerspectiveProj::SetUpProjection(UINT _screenWidth, UINT _screenHeight, 
    IGetRenderResource* _pRenderResource)
{
    // �r���[�ϊ��s����쐬����
    DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(
        fov,
        static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight),   // �A�X�y�N�g��
        nearZ,
        farZ);
    mat = XMMatrixTranspose(mat);

    // ���e�s��̎Q�Ƃ��擾���A�r���[�ϊ��s���������
    auto& cb = _pRenderResource->GetParameter().cbProjectionSet;
    XMStoreFloat4x4(&cb.data.projection, mat);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto pDeviceContext = _pRenderResource->GetDeviceContext();
    // CBuffer�ɂЂ��Â��n�[�h�E�F�A���\�[�X�}�b�v�擾�i���b�N���Ď擾�j
    HRESULT hr = pDeviceContext->Map(
        cb.pBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource);

    if (FAILED(hr))
    {
        MessageError("�r���[�ϊ��s��쐬�ŃG���[");
        return false;
    }

    CopyMemory(mappedResource.pData, &cb.data, sizeof(cb.data));
    // �}�b�v����
    pDeviceContext->Unmap(cb.pBuffer, 0);

    // VS��ProjectionMatrix���Z�b�g(������1�x�Z�b�g���ĈȌ�s��)
    pDeviceContext->VSSetConstantBuffers(2, 1, &cb.pBuffer);

    return true;
}
