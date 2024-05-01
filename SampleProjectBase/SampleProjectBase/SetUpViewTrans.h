#pragma once

class IGetRenderResource;

// ビュー変換行列を準備する
class SetUpViewTrans
{

public :
	SetUpViewTrans();
	~SetUpViewTrans();

	bool SetUpViewTransform(const DirectX::XMMATRIX& _viewMatrix, IGetRenderResource* _pRenderResource);
};

