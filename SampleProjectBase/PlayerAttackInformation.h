#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief プレイヤーの攻撃情報
	class PlayerAttackInformation : public AttackInformation
	{
		/// @brief ブレイク値
		float breakValue;
	public:
		/// @brief コンストラクタ
		/// @param _pAttacker 攻撃者
		PlayerAttackInformation(IAttacker* _pAttacker);
		~PlayerAttackInformation() {}

		/// @brief ブレイク値を取得する
		/// @return ブレイク値
		float GetBreakValue() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}


