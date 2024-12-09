#pragma once
#include "CharacterActState_Base.h"
#include "GameInput.h"
#include "CP_Camera.h"
#include "CP_RigidBody.h"
#include "CP_Animation.h"
#include "ITargetAccepter.h"

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
private:
	///// @brief この行動クラスのステートタイプ
	//PlayerState stateType;

	/// @brief ターゲットを見るときの回転速度
	float targetLookRotateSpeed;

	/// @brief 最後にキャンセル予約してからの経過時間
	float lastCancelReserveElapse;

	/// @brief ターゲット時に敵を見る行動にするか？
	bool isTargetLookAtEnemy;
protected:
	/// @brief キャンセルして繰り出す状態
	int actionReserveState;

	/// @brief キャンセルして繰り出す状態
	int atkReserveState;

	/// @brief キャンセルして繰り出す状態
	int moveReserveState;

	/// @brief プレイヤーアクションコントローラー
	PlayerActionController_Base* pActionController;

	/// @brief ゲーム入力クラス
	static GameInput* pPlayerInput;
public:
	PlayerActState_Base();
	virtual ~PlayerActState_Base() {}

	/// @brief 初期化処理
	/// @param _actController　プレイヤーコンポーネント
	void Init(PlayerActionController_Base& _actController);

	/// @brief 開始処理呼び出し
	void OnStart() override;

	/// @brief 更新処理呼び出し
	void Update() override;

	/// @brief  終了処理呼び出し
	void OnEnd() override;

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief 入力のフラグをクリア
	void ParameterClear();

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

	/// @brief キャンセルに合わせてアクションを変更するかチェック
	void CancelTransitionUpdate();
protected:
	// アニメーションコントローラ内のプレイヤー名
	constexpr static auto SPEEDRATIO_PARAMNAME{ "speed" };
	constexpr static auto MOVEAXIS_X_PARAMNAME{ "axisX" };	// X移動
	constexpr static auto MOVEAXIS_Y_PARAMNAME{ "axisY" };	// Y移動
	constexpr static auto DAMAGETRIGGER_PARAMNAME{ "damageTrigger" };	// 攻撃トリガー

	// アニメーション名
	constexpr static auto IDLE_ANIM_NAME{ "Idle" };	// 待機状態
};

