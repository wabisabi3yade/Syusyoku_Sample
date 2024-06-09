#pragma once

class IGetRenderResource;

// ƒrƒ…[•ÏŠ·s—ñ‚ğ€”õ‚·‚é
class SetUpViewTrans
{

public :
	SetUpViewTrans(){};
	~SetUpViewTrans(){};

	bool SetUpViewTransform(const DirectX::XMMATRIX& _viewMatrix, IGetRenderResource* _pRenderResource);
};

