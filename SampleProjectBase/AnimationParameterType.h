#pragma once

namespace HashiTaku
{
	namespace AnimParam	// �A�j���[�V�����p�����[�^
	{
		/// @brief �J�ڏ����Ɏg�p����ϐ��̌^��
		using conditionValType = std::variant<bool, float, int>;

		// �R���{�{�b�N�X�ō쐬����
		enum class TypeKind
		{
			Bool,
			Int,
			Float
		};
	}
}
