#pragma once
#include "PlayerAirState.h"
#include "PlayerAttackInformation.h"

namespace HashiTaku
{
	/// @brief プレイヤーの空中攻撃
	class PlayerAirAttack : public PlayerAirState
	{
		/// @brief コンビネーション攻撃の先
		PlayerState nextCombAtkState;

		/// @brief 攻撃情報リスト(単発なら最初の情報を使用する)
		std::vector<std::unique_ptr<PlayerAttackInformation>> attackInfos;

		/// @brief 全体の攻撃情報何個あるか
		u_int attackTimeCnt;
	public:
		PlayerAirAttack();
		~PlayerAirAttack() {}

	private:
		/// @brief 初期か処理
		void InitState() override;

		/// @brief 開始
		void OnStartBehavior() override;

		/// @brief 更新処理
		void UpdateBehavior() override;

		/// @brief 終了
		void OnEndBehavior() override;

		/// @brief アニメーション終了時の行動
		/// @param _fromAnimNodeName 遷移元のアニメーションノード名
		/// @param _toAnimNodeName 遷移先のアニメーションノード名
		void OnAnimationEnd(const std::string& _fromAnimNodeName,
			const std::string& _toAnimNodeName) override;

		/// @brief 攻撃情報を作成
		/// @return 攻撃情報
		std::unique_ptr<PlayerAttackInformation> CreateAttackInfo();

		/// @brief コンビネーション攻撃の入力更新
		void UpdateCombInput();

		/// @brief 敵の方向を見る
		void LookAtEnemy();

		/// @brief 攻撃情報を更新する
		void UpdateAttackInfo();

		/// @brief Y速度を0にして落ちないようにする
		void ClearVelocityY();
	};
}