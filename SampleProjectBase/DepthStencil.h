#pragma once
#include "Texture.h"

// �[�x�X�e���V���N���X
class DepthStencil : public Texture
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

public:
	DepthStencil() {}
	~DepthStencil() {}

	/// @brief �N���A����
	void Clear();

	/// @brief �쐬����
	/// @param _width ����
	/// @param _height �c��
	/// @param _useStencil 
	/// @return �쐬�����������H
	bool Create(UINT _width, UINT _height, bool _useStencil);

	ID3D11DepthStencilView* GetView() const;
};

