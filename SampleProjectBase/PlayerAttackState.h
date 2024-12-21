#pragma once
#include "PlayerGroundState.h"
#include "PlayerAttackInformation.h"
#include "AnimationCurve.h"

namespace HashiTaku
{
	/// @brief プレイヤー攻撃のゲーム内処理
	class PlayerAttackState : public PlayerGroundState
	{
	private:
		/// @brief コンビネーション攻撃の先
		PlayerState nextCombAtkState;

		/// @brief 攻撃情報リスト(単発なら最初の情報を使用する)
		std::vector<PlayerAttackInformation> attackInfos;

		/// @brief 前に進む距離カーブ
		AnimationCurve progressDistanceCurve;

		/// @brief リアタック変数
		const bool* pIsReAttack;

		/// @brief 今回の攻撃で進む距離
		float curAtkProgressDis;

		/// @brief 進む最大距離
		float atkMaxDistance;

		/// @brief 前フレームまでに進んだ距離
		float prevProgressDistance;

		/// @brief 攻撃情報何個目か
		u_int curAttackTime;

		/// @brief 全体の攻撃情報何個あるか
		u_int attackTimeCnt;

		/// @brief 前へ進むか？
		bool isMoveForward;
	public:
		PlayerAttackState();
		virtual ~PlayerAttackState() {}

		json Save() override;
		void Load(const json& _data) override;
	protected:
		// State共通処理
		void OnStartBehavior() override;
		void UpdateBehavior() override;
		void OnEndBehavior() override;

		void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

		/// @brief 攻撃回数をセット
		void SetAttackTimes(u_int _attackTimes);

		/// @brief 攻撃回数を変えたときの処理
		virtual void OnChangeAttackTimes();

		/// @brief 指定した攻撃情報を取得
		/// @param _atkIdx 配列の要素数
		/// @return 攻撃情報(範囲外ならnullptr)
		PlayerAttackInformation* GetPlayerAtkInfo(u_int _atkIdx);

		/// @brief 攻撃回数を取得
		/// @return 攻撃回数
		u_int GetAttackTimes() const;

		void ImGuiDebug() override;

		// 攻撃情報の編集
		virtual void ImGuiAttackInfo();
	private:
		/// @brief パラメータを初期化
		void InitParameter();

		/// @brief 攻撃情報を更新する
		void UpdateAttackInfo();

		/// @brief リアタックするか確認する
		void UpdateReAttack();

		/// @brief コンビネーション攻撃の入力更新
		void UpdateCombInput();

		/// @brief 攻撃で進む距離を求める
		/// @param _atkEnemyPos 敵の座標
		void CalcProgressDis(const DirectX::SimpleMath::Vector3& _atkEnemyPos);

		/// @brief 敵に対して向ける
		/// @param _atkEnemyPos 敵の座標
		void LookAtEnemyInstant(DirectX::SimpleMath::Vector3 _atkEnemyPos);

		/// @brief 攻撃時に前へ進む
		void ForwardProgressMove();

		void ImGuiCombAttack();
	private:
#ifdef EDIT
		// コンビネーション攻撃ができるステート
		inline static std::vector<std::string> combAtkState
		{
			"Attack11",
			"Attack12",
			"Attack13",
			"Attack14"
		};
#endif // EDIT
	};
}