#pragma once
#include "AnimationParameterType.h"

/// @brief �ϐ����Q�Ƃ��A�J�ڏ�����ύX������B�����Ă��邩�m�F����N���X
class TransCondition_Base : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief �Q�Ƃ��Ă���ϐ��̌^��
	HashiTaku::AnimParam::TypeKind typeKind;

protected:
	/// @brief �p�����[�^��
	const std::string* pParameterName;

public:
	TransCondition_Base(const std::string& _parameterName, HashiTaku::AnimParam::TypeKind _typeKind);
	virtual ~TransCondition_Base() {}

	/// @brief �J�ڏ����𖞂����Ă��邩�m�F
	/// @return �J�ڏ����������Ă��邩�H
	virtual bool IsCondition() const = 0;

	/// @brief �Q�Ƃ��Ă���p�����[�^�����擾����
	/// @return �Q�Ƃ��Ă���p�����[�^��
	const std::string& GetReferenceParamName() const;

	/// @brief �Ή����Ă���^�̎�ނ��擾����
	/// @return �^�̎��
	HashiTaku::AnimParam::TypeKind GetTypeKind() const;

	virtual nlohmann::json Save() = 0;
	virtual void Load(const nlohmann::json& _data) = 0;
protected:
	void ImGuiSetting() override;
};

