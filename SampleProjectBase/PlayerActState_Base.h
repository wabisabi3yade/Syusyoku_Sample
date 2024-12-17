#pragma once
#include "CharacterActState_Base.h"
#include "GameInput.h"
#include "CP_Camera.h"
#include "CP_RigidBody.h"
#include "CP_Animation.h"
#include "ITargetAccepter.h"
#include "PlayerAnimReference.h"

namespace HashiTaku
{
	class CP_Player;
	class PlayerActionController_Base;

	/// @brief プレイヤーの行動ステート基底クラス
	class PlayerActState_Base : public CharacterActState_Base
	{
	public:
		/// @brief プレイヤーから見た入力の方向
		enum class InputVector
		{
			Forward,	// 前方向
			Back	// 後ろ
		};

		/// @brief キャンセルの種類
		enum class CancelType
		{
			Action,	// アクション(ローリングなど)
			Attack,	// 攻撃
			Move,	// 移動
			MaxNum,
			None	// キャンセルしない
		};
	private:
		/// @brief キャンセルタイプ
		CancelType cancelType;

		/// @brief ターゲットを見るときの回転速度
		float targetLookRotateSpeed;

		/// @brief 行動が起きる優先順位(大きい方が優先)
		int statePriority;

		/// @brief ターゲット時に敵を見る行動にするか？
		bool isTargetLookAtEnemy;
	protected:
		/// @brief プレイヤーアクションコントローラー
		PlayerActionController_Base* pActionController;

		/// @brief ゲーム入力クラス
		static GameInput* pPlayerInput;
	public:
		PlayerActState_Base();
		virtual ~PlayerActState_Base() {}

		/// @brief 初期化処理
		/// @param _actController　プレイヤーコンポーネント
		/// @param _cancelType　キャンセルタイプ
		/// @param _priority　優先度
		void Init(PlayerActionController_Base& _actController, 
			CancelType _cancelType, 
			int _priority);

		/// @brief 更新処理呼び出し
		void Update() override;

		/// @brief キャンセルタイプを取得
		/// @return キャンセルタイプ
		CancelType GetCancelType() const;

		/// @brief 優先度を取得する
		/// @return 優先度
		int GetPriority() const;

		/// @brief セーブする
		/// @return セーブデータ
		nlohmann::json Save() override;

		/// @brief ロードする
		/// @param _data ロードするデータ 
		void Load(const nlohmann::json& _data) override;
	protected:
		/// @brief 速度をクリアする
		/// @param _applyY Y軸にも反映させるか
		void ClearVelocity(bool _applyY = true);

		/// @brief その行動はターゲットを見るのかセット
		/// @param _isLook 見るのか？
		void SetTargetAtEnemy(bool _isLook);

		/// @brief 無敵にするか？
		/// @param _isInvicible 無敵
		void SetInvicible(bool _isInvicible);

		/// @brief プレイヤーを取得する
		/// @return プレイヤー
		CP_Player& GetPlayer();

		/// @brief RigidBodyを取得
		/// @return RigidBody
		CP_RigidBody& GetRB();

		/// @brief プレイヤーのトランスフォームを取得
		/// @return トランスフォーム
		Transform& GetMyTransform();

		/// @brief アニメーションを取得する
		/// @return アニメーション
		CP_Animation* GetAnimation();

		/// @brief ターゲット先のポインタを取得
		/// @return ターゲット先のポインタ
		const ITargetAccepter* GetTargetAccepter();

		/// @brief Δtを取得
		/// @return Δt
		float DeltaTime() const;

		/// @brief Δtを進める速度を取得
		/// @return Δt進める速度
		float GetDeltaSpeed() const;

		/// @brief コントローラーの左スティックの入力を取得
		/// @return 左スティックの入力
		DirectX::SimpleMath::Vector2 GetInputLeftStick() const;

		/// @brief カメラから見たコントローラーの左スティックの入力を取得
		/// @return カメラから見た左スティックの入力(yにZ軸成分)
		DirectX::SimpleMath::Vector2 GetInputLeftStickFromCam() const;

		/// @brief 攻撃する敵の座標を取得する
		/// @return 敵の座標
		DirectX::SimpleMath::Vector3 GetAtkEnemyPos();

		/// @brief その方向に入力できているか確認する
		/// @param _checkVector 確認したい方向
		/// @return できているか？
		bool IsInputVector(InputVector _checkVector);

		/// @brief ローリング入力できているか？
		/// @return ローリングできるか？
		bool IsRollingInput() const;

		/// @brief 必殺技入力できているか？
		/// @param _inputVecter 方向の入力
		/// @return  必殺技できるか？
		bool IsSpecialAtkInput(InputVector _inputVecter);

		void ImGuiDebug() override {};
	private:
		/// @brief ターゲットの方向を見る
		void UpdateTargetLook();
	protected:
		// アニメーションコントローラ内のプレイヤー名
		constexpr static auto SPEEDRATIO_PARAMNAME{ "speed" };
		constexpr static auto MOVEAXIS_X_PARAMNAME{ "axisX" };	// X移動
		constexpr static auto MOVEAXIS_Y_PARAMNAME{ "axisY" };	// Y移動
		constexpr static auto DAMAGETRIGGER_PARAMNAME{ "damageTrigger" };	// 受けダメージトリガー
		constexpr static auto ATTACKTRIGGER_PARAMNAME{ "attackTrigger" };	// 攻撃トリガー
		constexpr static auto REATTACK_PARAMNAME{ "isReAttack" };	// リアタック時bool
		// キャンセルできるかを表すアニメーションパラメータ
		constexpr static auto CANCEL_PARAMNAME{ "canCancel" };

		// アニメーション名
		constexpr static auto IDLE_ANIM_NAME{ "Idle" };	// 待機状態
	};
}