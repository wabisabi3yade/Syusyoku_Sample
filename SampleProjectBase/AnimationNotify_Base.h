#pragma once
#include "IClassNameGetter.h"

/// @brief �A�j���[�V�����̒ʒm�C���^�[�t�F�[�X
class AnimationNotify_Base : public HashiTaku::IImGuiUser, public ISaveLoad
{
	/// @brief �������
	bool isActive;

	/// @brief �C�x���g�̖��O
	std::string notifyName;
public:
	AnimationNotify_Base();
	virtual ~AnimationNotify_Base() {}

	/// @brief �A�j���[�V�����̒ʒm�̍X�V
	/// @param _lastPlayingRatio �A�j���[�V�����̑O��̍Đ�����
	/// @param _curPlayingRatio �A�j���[�V�����̌��݂̍Đ�����
	/// @param _isLoop ���������[�v�������H
	virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) = 0;

	// ������Ԃ��Z�b�g
	void SetIsActive(bool _isActive);

	// �C�x���g�����擾
	void SetNotifyName(const std::string& _notifyName);

	// ������Ԃ��擾
	bool GetIsActive() const;

	// �C�x���g���擾
	std::string GetNotifyName() const;

	// �^�����擾����
	virtual std::string GetTypeName() const = 0;

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

