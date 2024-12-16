#pragma once
#include "PlayerAttackState.h"

namespace HashiTaku
{
	class ITargetAccepter;

	/// @brief 前突進攻撃
	class PlayerRushAttack : public PlayerAttackState
	{
		/// @brief 横移動カーブ
		AnimationCurve distanceHoriCurve;

		/// @brief ターゲット先のオブジェクト
		const ITargetAccepter* pTargetObj;

		/// @brief 横移動する距離の長さ
		float progressLengthHori;

		/// @brief 前回フレームまでの横移動距離
		float prevDistanceHori;

		/// @brief 斬り始める敵との距離
		float slashBeginDistance;

		/// @brief キャンセルアニメーション割合
		float cancelBeginRatio;

		/// @brief 斬り始めるアニメーション割合
		float slashBeginAnimRatio;

		/// @brief 前に進むか
		bool isProgressForward;
	public:
		PlayerRushAttack();
		~PlayerRushAttack() {}

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		// State共通処理
		void OnStartBehavior() override;
		void UpdateBehavior() override;
		void TransitionCheckUpdate();

		/// @brief 前方向に突進
		void MoveForward();

		/// @brief 敵との距離が一定距離いかになったら斬り終える
		bool CheckDistanceToEnemy(float _currentPlayRatio);

		/// @brief ターゲット先が存在するか取得
		/// @return ターゲット先が存在するか？
		bool isExistTrgeter() const;

		void ImGuiDebug() override;
	};
}