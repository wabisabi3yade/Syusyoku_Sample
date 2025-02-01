#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief 敵側の攻撃情報
	class BossAttackInformation : public AttackInformation
	{
		/// @brief パリィ時に追加するブレイク値
		float addBreakValueOnParry;
	public:
		BossAttackInformation();
		~BossAttackInformation() {}

		/// @brief パリィ時に加算するブレイク値を取得
		/// @return パリィ時に加算するブレイク値
		float GetBreakValueOnParry() const;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void ImGuiDebug() override;
	};
}

