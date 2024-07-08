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
   _pRenderResource->GetParameter().SetProjection(mat);
    return true;
}
