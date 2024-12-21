#pragma once
#include "AnimationParameterType.h"

namespace HashiTaku
{
	/// @brief �J�ڏ�����ύX������B�����Ă��邩�m�F����N���X
	class TransCondition_Base : public IImGuiUser, public ISaveLoad
	{
		/// @brief �Q�Ƃ��Ă���ϐ��̌^��
		AnimParam::TypeKind typeKind;

	protected:
		/// @brief �p�����[�^��
		const std::string* pParameterName;

	public:
		TransCondition_Base(const std::string& _parameterName, AnimParam::TypeKind _typeKind);
		virtual ~TransCondition_Base() {}

		/// @brief �J�ڏ����𖞂����Ă��邩
		/// @param _checkValue �m�F����l
		/// @return �J�ڏ����������Ă��邩�H
		virtual bool IsCondition(AnimParam::conditionValType _checkValue) const = 0;

		/// @brief ���O�̎Q�Ƃ��Z�b�g����
		/// @param _paramRefarenceName 
		void SetReferenceParamName(const std::string& _paramRefarenceName);

		/// @brief �Q�Ƃ��Ă���p�����[�^�����擾����
		/// @return �Q�Ƃ��Ă���p�����[�^��
		const std::string& GetReferenceParamName() const;

		/// @brief �Ή����Ă���^�̎�ނ��擾����
		/// @return �^�̎��
		AnimParam::TypeKind GetTypeKind() const;

		virtual json Save() = 0;
		virtual void Load(const json& _data) = 0;
	protected:
		void ImGuiDebug() override;
	};
}