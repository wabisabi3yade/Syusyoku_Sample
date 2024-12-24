#pragma once
#include "AnimationNotifyEvent.h"
#include "ITransformSetter.h"

namespace HashiTaku
{
	/// @brief �A�j���[�V�����̃^�C�~���O�ŃG�t�F�N�g���o���ʒm�C�x���g
	class ANE_CreateVfx : public AnimationNotifyEvent, public ITransformSetter
	{
		/// @brief �G�t�F�N�g���
		CreateVfxInfo vfxInfo;

		/// @brief �g�����X�t�H�[������̃G�t�F�N�g���o���I�t�Z�b�g
		DXSimp::Vector3 createOffsetPos;

		/// @brief �g�����X�t�H�[�����
		const Transform* pTransform;

		/// @brief �G�t�F�N�g�̃n���h��
		Effekseer::Handle vfxHandle;

		/// @brief �ʒm�C�x���g���؂�ւ�����Ƃ��ɒ����ɃG�t�F�N�g���������H
		bool isExitDestroy;
	public:
		ANE_CreateVfx();
		~ANE_CreateVfx() {}
		
		/// @brief �N���[���֐�
		/// @return �ʒm�N���X
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		/// @brief �g�����X�t�H�[���̃Z�b�^�[
		/// @param _transform �g�����X�t�H�[��
		void SetTransform(const Transform& _transform) override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �C�x���g�֐�
		void OnEvent() override;

		/// @brief �؂�ւ��O����
		void OnTerminal() override;

		void ImGuiDebug() override;
	};
}


