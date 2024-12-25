#pragma once

namespace HashiTaku
{
	class GameObject;

	/// @brief �A�j���[�V�����̒ʒm�C���^�[�t�F�[�X
	class AnimationNotify_Base : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief �ʒm�C�x���g���
		enum class NotifyType
		{
			// �f�o�b�O���O
			ANE_DebugLog,
			ANS_DebugLog,

			// �p�����[�^�ύX
			ANE_ChangeParameter,
			ANS_ChangeParameter,

			// �G�t�F�N�g�쐬
			ANE_CreateVfx,

			// �J�����V�F�C�N
			ANE_CameraShake,

			// �p�b�h�U��
			ANE_PadShake,

			None
		};

	private:
		/// @brief �������
		bool isActive;

		/// @brief �C�x���g�̖��O
		std::string notifyName;

		/// @brief �C�x���g���
		NotifyType notifyType;

		/// @brief �A�j���[�V�����̃t���[����
		u_int animFrameCnt;
#ifdef EDIT
		bool isEditing{ false };
#endif // EDIT

	public:
		AnimationNotify_Base();

		/// @brief �R���X�g���N�^
		/// @param _notifyType �ʒm�C�x���g���
		AnimationNotify_Base(NotifyType _notifyType);
		virtual ~AnimationNotify_Base() {}

		void Init();

		/// @brief �A�j���[�V�����̒ʒm�̍X�V
		/// @param _lastPlayingRatio �A�j���[�V�����̑O��̍Đ�����
		/// @param _curPlayingRatio �A�j���[�V�����̌��݂̍Đ�����
		/// @param _isLoop ���������[�v�������H
		virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, const bool _isLoop) = 0;

		/// @brief �ʒm�C�x���g���n�܂����Ƃ��̏�����
		void OnInitCall();

		/// @brief  ���̃A�j���[�V�����ɐ؂�ւ���O�̏I�������Ăяo���֐�
		void OnTerminalCall();

		// ������Ԃ��Z�b�g
		void SetIsActive(bool _isActive);

		// �C�x���g�����擾
		void SetNotifyName(const std::string& _notifyName);

		/// @brief �A�j���[�V�����̃t���[�������Z�b�g
		void SetAnimationFrameCnt(u_int _animFrame);

		/// @brief ��ނ��疼�O�ɕϊ�
		/// @param _notifyType ���
		/// @return ���O
		static std::string GetTypeToStr(NotifyType _notifyType);

		// ������Ԃ��擾
		bool GetIsActive() const;

		// �C�x���g���擾
		std::string GetNotifyName() const;

		/// @brief �ʒm�C�x���g�̎�ނ��擾����
		/// @return �ʒm�C�x���g�̎��
		NotifyType GetNotifyType() const;

		/// @brief �S�̂̃t���[�������擾
		/// @return �t���[����
		u_int GetAnimFrameCnt() const;

		/// @brief �N���[���֐�
		/// @return ���������ʒm�C�x���g
		virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

		json Save() override;
		void Load(const json& _data) override;

	protected:
		/// @brief �n�܂����Ƃ��̊J�n����
		virtual void OnInitialize() {};

		/// @brief ���̃A�j���[�V�����ɐ؂�ւ���O�̏I������
		virtual void OnTerminal() {};

		/// @brief �t���[�������犄���ɕϊ�
		/// @param _frame �t���[����
		/// @return ����
		float ConvertFrameToRatio(u_int _frame);

		/// @brief �t���[�����Œ������邩�擾
		/// @return �t���[�����Œ������邩�H
		bool UseFrame();


		void ImGuiDebug() override;
	};

	// �R���Z�v�g�Ŏ擾�ł���
	template<typename T>
	concept AnimNotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;

}