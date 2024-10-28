#pragma once

class GameObject;

/// @brief �A�j���[�V�����̒ʒm�C���^�[�t�F�[�X
class AnimationNotify_Base : public HashiTaku::IImGuiUser, public  HashiTaku::ISaveLoad
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

		None
	};

private:
	/// @brief �������
	bool isActive;

	/// @brief �C�x���g�̖��O
	std::string notifyName;

	/// @brief �C�x���g���
	NotifyType notifyType;
#ifdef EDIT
	bool isEditing{ false };
#endif // EDIT

public:
	AnimationNotify_Base();
	AnimationNotify_Base(NotifyType _notifyType);
	virtual ~AnimationNotify_Base() {}

	void Init();

	/// @brief �A�j���[�V�����̒ʒm�̍X�V
	/// @param _lastPlayingRatio �A�j���[�V�����̑O��̍Đ�����
	/// @param _curPlayingRatio �A�j���[�V�����̌��݂̍Đ�����
	/// @param _isLoop ���������[�v�������H
	virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) = 0;

	/// @brief ���̒ʒm�C�x���g�ɐ؂�ւ���O�̏I������
	virtual void OnTerminal() {};

	// ������Ԃ��Z�b�g
	void SetIsActive(bool _isActive);

	// �C�x���g�����擾
	void SetNotifyName(const std::string& _notifyName);

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

	/// @brief �N���[���֐�
	/// @return ���������ʒm�C�x���g
	virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

protected:
	void ImGuiSetting() override;
};

namespace HashiTaku
{
	// �R���Z�v�g�Ŏ擾�ł���
	template<typename T>
	concept AnimNotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;
}

