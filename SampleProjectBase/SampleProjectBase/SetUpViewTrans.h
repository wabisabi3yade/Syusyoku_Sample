#pragma once

class IGetRenderResource;

// �r���[�ϊ��s�����������
class SetUpViewTrans
{

public :
	SetUpViewTrans(){};
	~SetUpViewTrans(){};

	bool SetUpViewTransform(const DirectX::XMMATRIX& _viewMatrix, IGetRenderResource* _pRenderResource);
};

