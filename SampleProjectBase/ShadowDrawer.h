#pragma once
#include "RenderTarget.h"
#include "DepthStencil.h"

namespace HashiTaku
{
	/// @brief �e�`��N���X
	class ShadowDrawer : public Singleton_Base<ShadowDrawer>
	{
		friend class Singleton_Base<ShadowDrawer>;

		/// @brief �[�x���������ރ����_�[�^�[�Q�b�g
		std::unique_ptr<RenderTarget> pRenderTarget;

		/// @brief �[�x�X�e���V��
		std::unique_ptr<DepthStencil> pDepthStencil;
	public:
		/// @brief �e�`��N���X������
		void Init();
	private:
		ShadowDrawer() {}
		~ShadowDrawer() {}
	};
}