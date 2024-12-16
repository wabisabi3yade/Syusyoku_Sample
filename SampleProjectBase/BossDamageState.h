#pragma once
#include "BossActState_Base.h"

namespace HashiTaku
{
	/// @brief ボスダメージくらいステート
	class BossDamageState : public BossActState_Base
	{

	public:
		BossDamageState();
		virtual ~BossDamageState() {}

	protected:
		/// @brief 開始処理
		void OnStartBehavior() override;

		/// @brief アニメーション終了時の行動
		/// @param _fromAnimNodeName 遷移元のアニメーションノード名
		/// @param _toAnimNodeName 遷移先のアニメーションノード名
		void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

	private:
		/// @brief ダメージ時プレイヤーに向ける
		void LookPlayer();
	};
}