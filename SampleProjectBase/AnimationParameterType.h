#pragma once
#include "TriggerType.h"

namespace HashiTaku
{
	namespace AnimParam	// �A�j���[�V�����p�����[�^
	{
		/// @brief �J�ڏ����Ɏg�p����ϐ��̌^��
		using conditionValType = std::variant<bool, float, int, TriggerType>;

		template<typename T>
		concept AnimParamConcept = std::same_as<T, bool> || std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, TriggerType>;

		// �R���{�{�b�N�X�ō쐬����
		enum class TypeKind
		{
			Bool,
			Int,
			Float,
			Trigger
		};
	}
}
