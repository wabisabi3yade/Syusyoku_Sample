#pragma once
#include "AnimationParameterType.h"

/// @brief �J�ڏ�����ύX������B�����Ă��邩�m�F����N���X
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

	/// @brief �J�ڏ����𖞂����Ă��邩
	/// @param _checkValue �m�F����l
	/// @return �J�ڏ����������Ă��邩�H
	virtual bool IsCondition(HashiTaku::AnimParam::conditionValType _checkValue) const = 0;

	/// @brief ���O�̎Q�Ƃ��Z�b�g����
	/// @param _paramRefarenceName 
	void SetReferenceParamName(const std::string& _paramRefarenceName);

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

