#pragma once
#include "AnimationNotify_Base.h"

namespace HashiTaku
{
	/// @brief �A�j���[�V�����̒ʒm
	class AnimationNotifyState : public AnimationNotify_Base
	{
		/// @brief �J�n�C�x���g����
		float startEventRatio;
		/// @brief �J�n�C�x���g�t���[��
		u_int startEventFrame;

		/// @brief �I���C�x���g����
		float endEventRatio;
		/// @brief �I���C�x���g�t���[��
		u_int endEventFrame;

		/// @brief ���݂̍Đ�����
		float curPlayRatio;

		/// @brief 1�t���[���O�̂̍Đ�����
		float lastPlayRatio;
	public:
		/// @brief �R���X�g���N�^
		/// @param _notifyType �ʒm�C�x���g���
		AnimationNotifyState(NotifyType _notifyType);
		virtual ~AnimationNotifyState() {}

		void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) override;

		// �J�n���̃C�x���g�������Z�b�g����
		void SetStartRatio(float _startRatio);

		// �I�����̃C�x���g�������Z�b�g����
		void SetEndRatio(float _endRatio);

		/// @brief �N���[���֐�
		/// @return ���������ʒm�C�x���g
		virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

		json Save() override;
		void Load(const json& _data) override;
	protected:
		/// @brief �C�x���g�J�n����
		virtual void Begin() = 0;

		/// @brief �C�x���g�Ԃ̍X�V����
		virtual void Tick() = 0;

		/// @brief �C�x���g�I������
		virtual void End() = 0;

		/// @brief �J�n�̃C�x���g�������擾
		/// @return �J�n�C�x���g����
		float GetStartEventRatio() const;

		/// @brief �I���̃C�x���g�������擾
		/// @return �I���C�x���g����
		float GetEndEventRatio() const;

		/// @brief ���݂̍Đ��������擾����
		/// @return ���݂̊���
		float GetCurrentRatio() const;

		/// @brief 1�t���[���O�̍Đ��������擾
		/// @return 1�t���[���O�̊���
		float GetLastRatio() const;

		void ImGuiDebug() override;
	};
}