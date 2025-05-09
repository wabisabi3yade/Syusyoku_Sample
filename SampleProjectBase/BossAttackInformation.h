#pragma once
#include "AttackInformation.h"
#include "IAttacker_AcceptParry.h"

namespace HashiTaku
{
	/// @brief 敵側の攻撃情報
	class BossAttackInformation : public AttackInformation
	{
		/// @brief パリィ時に追加するブレイク値
		float addBreakValueOnParry;

		/// @brief パリィされたらよろめくようにするか？
		bool isStunOnParry;
	public:
		BossAttackInformation(IAttacker* _pAttacker);
		~BossAttackInformation() {}

		/// @brief パリィされたことを通知する
		/// @param _acceptInfo パリィからの情報
		void NotifyAcceptParry(const AcceptParryInfo& _acceptInfo);

		/// @brief パリィ時に加算するブレイク値を取得
		/// @return パリィ時に加算するブレイク値
		float GetBreakValueOnParry() const;

		/// @brief パリィされたらよろめくか取得
		/// @return パリィされたらよろめくか？
		bool GetIsStunOnParry() const;

		json Save() override;
		void Load(const json& _data) override;

	private:
		IAttacker_AcceptParry* CastAcceptParry();

		void ImGuiDebug() override;
	};
}

