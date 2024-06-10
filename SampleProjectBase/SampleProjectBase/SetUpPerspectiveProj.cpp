#include "pch.h"
#include "SetUpPerspectiveProj.h"

#include "RenderParam.h"

// デフォルト値
constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

SetUpPerspectiveProj::SetUpPerspectiveProj()
{
    fov = DEFAULT_FOV;	// 視野角
    nearZ = DEFAULT_NEARZ;	// 描画距離（最短）
    farZ = DEFAULT_FARZ;	// 描画距離(最長)
}

SetUpPerspectiveProj::~SetUpPerspectiveProj()
{
}

bool SetUpPerspectiveProj::SetUpProjection(UINT _screenWidth, UINT _screenHeight, 
    IGetRenderResource* _pRenderResource)
{
    // ビュー変換行列を作成する
    DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(
        fov,
        static_cast<float>(_screenWidth) / static_cast<float>(_screenHeight),   // アスペクト比
        nearZ,
        farZ);
    mat = XMMatrixTranspose(mat);

    // 投影行列の参照を取得し、ビュー変換行列を代入する
   _pRenderResource->GetParameter().SetProjection(mat);
    return true;
}
