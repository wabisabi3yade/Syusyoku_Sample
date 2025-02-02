#pragma once
#include "IAttacker.h"
#include "IParryAccepter.h"

namespace HashiTaku
{
	/// @brief �U������I�u�W�F�N�g�B�p���B���ꂽ�Ƃ��ɏ������N�����C���^�[�t�F�[�X
	class IAttacker_AcceptParry : public IAttacker, public IParryAccepter
	{

	public:
		IAttacker_AcceptParry() = default;
		virtual ~IAttacker_AcceptParry() = default;
	};
}


