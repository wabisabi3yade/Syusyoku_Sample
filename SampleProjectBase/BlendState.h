#pragma once
#include "IGetRenderPara.h"

namespace HashiTaku
{
	// ���������������邽�߂̃u�����h�I�u�W�F�N�g
	class BlendState : public IGetRenderPara<ID3D11BlendState*>
	{
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendAlpha;

	public:
		// �����F�f�o�C�X
		BlendState() {};
		~BlendState() {};

		// ������
		bool Init(ID3D11Device& _pDevice);

		// �u�����h�X�e�[�g�I�u�W�F�N�g��n��
		ID3D11BlendState* GetParaObject() override { return pBlendAlpha.Get(); }
	};
}