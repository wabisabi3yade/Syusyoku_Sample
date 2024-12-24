#pragma once
#include "CharacterActState_Base.h"

namespace HashiTaku
{
	/// @brief �G�̃A�N�V�����X�e�[�g���
	class EnemyActState_Base : public CharacterActState_Base
	{

	public:
		EnemyActState_Base();
		virtual ~EnemyActState_Base() {}

		virtual json Save() = 0;
		virtual void Load(const json& _data) = 0;

	protected:
		virtual void ImGuiDebug() = 0;
	};
}