#pragma once

#include "CharacterActState_Base.h"
#include "GameInput.h"
#include "CP_Camera.h"
#include "CP_RigidBody.h"

class PlayerActionController;

/// @brief プレイヤーの行動ステート基底クラス
class PlayerActState_Base : public CharacterActState_Base
{
public:
	/// @brief  プレイヤーの行動状態
	enum class PlayerState
	{
		// 待機
		Idle,
		
		// 移動
		Move = 10,
		TargetMove,
		Jump,
		Rolling,

		// 地上コンビネーション攻撃
		Attack11 = 20,
		Attack12,
		Attack13,
		Attack14,

		// 地上必殺攻撃
		SpecialAtkHi = 30,
		SpecialAtkLow,

		// 最後
		None = 99
	};

	/// @brief プレイヤーから見た入力の方向
	enum class InputVector
	{
		Forward,	// 前方向
		Back	// 後ろ
	};

#ifdef EDIT
	// ステート一覧の文字列
	inline static const std::vector<std::string> playerStateNameList =
	{
		// 待機
		"Idle",

		// 移動
		"Move",
		"TargetMove",
		"Jump",
		"Rolling"

		// 地上コンビネーション攻撃
		"Attack11",
		"Attack12",
		"Attack13",
		"Attack14",

		// 地上必殺攻撃
		"SpecialAtkHi",
		"SpecialAtkLow",

		// 最後
		"None"
	};
#endif // EDIT

private:
	/// @brief この行動クラスのステートタイプ
	PlayerState stateType;

	/// @brief キャンセルして繰り出す状態
	PlayerState cancelPlayState;

	/// @brief ターゲットを見るときの回転速度
	float targetLookRotateSpeed;

	/// @brief 入力期間中に攻撃が押されていたか?
	bool isAttackInput;

	/// @brief ターゲット時に敵を見る行動にするか？
	bool isTargetLookAtEnemy;
protected:
	/// @brief プレイヤー
	PlayerActionController* pActionController;

	/// @brief ゲーム入力クラス
	static GameInput* pPlayerInput;

	/// @brief カメラクラス
	static CP_Camera* pCamera;
public:
	PlayerActState_Base();
	virtual ~PlayerActState_Base() {}

	/// @brief 初期化処理
	/// @param _stateType　状態
	/// @param _actController　プレイヤーコンポーネント
	/// @param _changeObserver ステート遷移オブザーバー
	void Init(PlayerState _stateType, PlayerActionController& _actController);

	/// @brief 開始処理呼び出し
	void OnStart() override;

	/// @brief 更新処理呼び出し
	void Update() override;

	/// @brief  終了処理呼び出し
	void OnEnd() override;

	// ステートタイプを取得
	PlayerState GetActStateType() const;

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief 各状態の開始処理
	virtual void OnStartBehavior() {};

	/// @brief 更新処理
	virtual void UpdateBehavior() {};

	/// @brief 各状態の終了処理
	virtual void OnEndBehavior() {};

	/// @brief ステート遷移条件のチェック処理
	virtual void TransitionCheckUpdate();

	/// @brief 入力のフラグをクリア
	void ParameterClear();

	/// @brief 入力確認の更新
	virtual void CheckInputUpdate();

	/// @brief 状態を遷移する
	/// @param _changeSate 遷移先の状態
	void ChangeState(PlayerState _nextState);

	/// @brief その行動はターゲットを見るのかセット
	/// @param _isLook 見るのか？
	void SetTargetAtEnemy(bool _isLook);

	/// @brief RigidBodyを取得
	/// @return RigidBody
	CP_RigidBody& GetRB();

	/// @brief プレイヤーのトランスフォームを取得
	/// @return トランスフォーム
	Transform& GetTransform();

	/// @brief Δtを取得
	/// @return Δt
	float DeltaTime() const;

	/// @brief Δtを進める速度を取得
	/// @return Δt進める速度
	float GetDeltaSpeed() const;

	/// @brief コントローラーの左スティックの入力を取得
	/// @return 左スティックの入力
	DirectX::SimpleMath::Vector2 GetInputLeftStick() const;

	/// @brief その方向に入力できているか確認する
	/// @param _checkVector 確認したい方向
	/// @return できているか？
	bool IsInputVector(InputVector _checkVector) const;

	/// @brief ローリング入力できているか？
	/// @return ローリングできるか？
	bool IsRollingInput() const;

	/// @brief 必殺技入力できているか？
	/// @param _inputVecter 方向の入力
	/// @return  必殺技できるか？
	bool IsSpecialAtkInput(InputVector _inputVecter) const;

	/// @brief 攻撃できるか取得する
	/// @return 攻撃できるか？
	bool GetCanCombAttack() const;

	/// @brief ImGui処理
	void ImGuiDebug() override;

	/// @brief ImGuiによるコンボボックス
	/// @param _caption キャプション
	/// @param _currentState 現在のステート
	/// @return 変更したか？
	static bool ImGuiComboPlayerState(const std::string& _caption, PlayerState& _currentState);

private:
	/// @brief ターゲットの方向を見る
	void UpdateTargetLook();

	/// @brief キャンセルに合わせてアクションを変更するかチェック
	void CancelTransitionUpdate();
protected:
	// アニメーションコントローラ内のプレイヤー名
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// 移動速度
	constexpr static auto MOVEAXIS_X_PARAMNAME = "axisX";	// 移動速度
	constexpr static auto MOVEAXIS_Y_PARAMNAME = "axisY";	// 攻撃トリガー
};

