#pragma once
#include "TransCondition_Base.h"
#include "AnimationParameterType.h"

namespace HashiTaku
{
	/// @brief �������쐬����N���X
	class TransConditionCreater
	{
	public:
		/// @brief �J�ڏ������쐬����
		/// @param _parameterValue �J�ڏ����p�����[�^
		/// @param _parameterName �p�����[�^��
		/// @return �쐬�����J�ڏ����C���X�^���X
		static std::unique_ptr<TransCondition_Base> Create(const AnimParam::conditionValType& _parameterValue, const std::string& _parameterName);
	};
}