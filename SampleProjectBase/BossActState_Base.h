#pragma once
#include "EnemyActState_Base.h"
#include "CP_Animation.h"
#include "CP_RigidBody.h"

namespace HashiTaku
{
	class BossActionController;

	/// @brief ボスのステートノード基底
	class BossActState_Base : public EnemyActState_Base
	{
	protected:
		/// @brief  ワープモーションで必要なパラメータ
		struct WarpMotionParam
		{
			/// @brief ワープモーションで移動する割合(XZ成分)
			std::unique_ptr<AnimationCurve> pHoriMovementCurve;

			/// @brief 移動するXZ最大距離(0未満なら制限なしとする)
			float maxMovementXZ{ -10.0f };

			/// @brief ターゲットとする座標からの差分へ向かうようにするオフセット
			float targetPosOffset{ 0.0f };

			/// @brief 開始する割合
			float beginAnimRatio{ 0.0f };

			/// @brief 終了する割合
			float endAnimRatio{ 1.0f };

			/// @brief ルートモーションの割合から移動割合を求める
			bool isFromRootMotion{ true };

			/*/// @brief ワープモーションで移動する割合(Y成分)
			AnimationCurve vertMovementCurve;*/

			/*	/// @brief 移動するY最大距離(0未満なら制限なしとする)
			float maxMovementY{ -10.0f };*/

			/*/// @brief Y移動させるか？
			bool isUseVertical{ false };*/
		};

	public:
		// ボスのステートタイプ(大体追加順)
		enum class BossState
		{
			// 移動
			Idle,
			Walk,
			Run,

			// 振る舞い
			Damage_Small,	// 小ダメージ
			BreakEnd_Knock,	// ブレイク終了ノック
			Break_Idle,	// ブレイク時待機

			// 攻撃
			CombAttack1 = 100,	// コンビネーション攻撃
			JumpAttack,	// ジャンプ攻撃
			SlidingAttack,	// スライディング攻撃
			CombAttack2,	// コンビネーション攻撃
			CombAttack3,	// コンビネーション攻撃

			// 最後
			None
		};

	private:
		/// @brief ワープモーションのパラメータ郡
		std::vector<WarpMotionParam> warpMotionParams;

		/// @brief ワープモーションのターゲット先のポインタ
		const DirectX::SimpleMath::Vector3* pWarpTargetPos;

		/// @brief ワープモーションのターゲット先座標
		DirectX::SimpleMath::Vector3 warpTargetPos;

		/// @brief ワープモーションで移動するときのターゲット座標との距離
		DirectX::SimpleMath::Vector3 disToWarpTargePos;

		/// @brief 最後のワープモーションで求めた進行度(0.0～1.0)
		float lastProgressRatio;

		/// @brief ボスのステート
		BossState stateType;

		/// @brief 全てで何回ワープさせるか？
		u_int allWarpSteps;

		/// @brief 現在のワープ回数
		u_int curWarpStep;

		/// @brief ワープ開始時のルートモーション座標
		DirectX::SimpleMath::Vector3 startRMPos;

		/// @brief ワープ終了時のルートモーション座標
		DirectX::SimpleMath::Vector3 endRMPos;

		/// @brief ワープモーションさせるか？
		bool isUseWarpMotion;

		/// @brief ワープモーション中か？
		bool isWarpMoving;

#ifdef EDIT
		// ワープモーション開始時の座標
		DirectX::SimpleMath::Vector3 warpStartPos;
#endif
	protected:
		/// @brief アクションコントローラー
		BossActionController* pActionController;
	public:
		BossActState_Base();
		virtual ~BossActState_Base() {}

		/// @brief 初期化処理
		/// @param _stateType　状態
		/// @param _actController　アクションコントローラー
		void Init(BossState _stateType, BossActionController& _actController);

		/// @brief 状態切り替え開始処理
		void OnStart() override;

		/// @brief 更新処理
		void Update() override;

		/// @brief 状態切り替え終了処理
		void OnEnd() override;

		/// @brief ダメージ時処理
		virtual void OnDamage();

		/// @brief デバッグ描画
		void DebugDisplay();

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	protected:
		/// @brief 状態切り替え開始処理
		virtual void OnStartBehavior() {}

		/// @brief 更新処理
		virtual void UpdateBehavior() {}

		/// @brief 状態切り替え終了処理
		virtual void OnEndBehavior() {}

		/// @brief 遷移切り替え確認
		virtual void TransitionCheckUpdate();

		/// @brief 遷移する
		/// @param _nextState 次の遷移
		/// @param _isForce　強制切り替え
		void ChangeState(BossState _nextState, bool _isForce = false);

		/// @brief ワープモーションのターゲットの座標の参照をセット
		/// @param _targetWorldPos ターゲットの座標の参照
		void SetWarpTargetPosReference(const DirectX::SimpleMath::Vector3& _targetPosRef);

		/// @brief ワープモーションのターゲットの座標をセット
		/// @param _targetWorldPos ターゲットの座標
		void SetWarpTargetPos(const DirectX::SimpleMath::Vector3& _targetPos);

		/// @brief トランスフォームを取得する
		/// @return ボスのトランスフォーム
		Transform& GetBossTransform();

		/// @brief トランスフォームを取得する
		/// @return プレイヤーのトランスフォーム
		Transform& GetPlayerTransform();

		/// @brief アニメーションコントローラポーネントを取得
		/// @return コンポーネント
		CP_Animation* GetAnimation();

		/// @brief Rbを取得
		/// @return Rbコンポーネント
		CP_RigidBody& GetRB();

		/// @brief 経過時間を取得する
		/// @return 経過時間
		float DeltaTime() const;

		void ImGuiDebug() override;
	private:
		/// @brief ワープモーションによる更新処理
		void WarpMotionUpdate();

		/// @brief 次のワープモーションに入るか移行するか確認する
		/// @param _animRatio 次のアニメーション割合
		void CheckTransNextWarp(float _animRatio);

		/// @brief ワープモーションのターゲットの座標をセット
		/// @param _targetWorldPos ターゲットの座標
		void CalcWarpDistance(const DirectX::SimpleMath::Vector3& _targetWorldPos);

		/// @brief ブレイク終了ノックができるか取得する
		/// @return ブレイク終了ノックできるか？
		bool GetCanBreakEndKdnock();

		// ワープモーションに関するImGui編集
		void ImGuiWarpDebug();

		// ワープモーション関連のセーブ
		nlohmann::json SaveWarpParameters();

		// ワープモーション関連のロード
		void LoadWarpParameters(const nlohmann::json& _warpData);
	public:
		// パラメータ
		/// @brief 移動速度のアニメーションパラメータ名
		static constexpr auto SPEED_ANIMPARAM_NAME{ "speed" };
		/// @brief 攻撃トリガーのアニメーションパラメータ名
		static constexpr auto ATTACKTRIGGER_ANIMPARAM_NAME{ "attackTrigger" };
		/// @brief 攻撃トリガーのアニメーションパラメータ名
		static constexpr auto DAMAGETRIGGER_ANIMPARAM_NAME{ "damageTrigger" };
		// リアタックするときに呼び出す変数
		static constexpr auto REATTACK_PARAMNAME{ "isReAttack" };

		// アニメーション
		/// @brief 待機状態のアニメーション名
		static constexpr auto IDLE_ANIM_NAME{ "Idle" };
		/// @brief 走り状態のアニメーション名
		static constexpr auto RUN_ANIM_NAME{ "Move" };
	};
}