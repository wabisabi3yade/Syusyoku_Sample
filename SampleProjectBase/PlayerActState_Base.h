#pragma once

#include "StateMachine.h"
#include "GameInput.h"
#include "IObserever.h"
#include "CP_Camera.h"
#include "CP_Animation.h"

class PlayerActionController;

/// @brief プレイヤーの行動ステート基底クラス
class PlayerActState_Base : public HashiTaku::StateNode_AnimationBase , public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
public:
	// プレイヤーの行動状態
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

	/// @brief 地上で行うアクションか？
	bool isGroundAction;
protected:
	/// @brief アニメーション管理
	CP_Animation* pAnimation;

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

	// アニメーションをセットする
	void SetAnimation(CP_Animation& _pAnimation);

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

	/// @brief 状態を遷移する
	/// @param _changeSate 遷移先の状態
	void ChangeState(PlayerState _nextState);

	/// @brief コントローラーの左スティックの入力を取得
	/// @return 左スティックの入力
	DirectX::SimpleMath::Vector2 GetInputLeftStick() const;

	/// @brief 地上で行うアクションか取得
	/// @return 地上で行うアクションなのか？
	bool GetIsGroundAction() const;

	/// @brief ローリングできるか取得する
	/// @return ローリングできるか？
	bool GetCanRolling() const;

	/// @brief ImGui処理
	virtual void ImGuiSetting() {}

	/// @brief ImGuiによるコンボボックス
	/// @param _caption キャプション
	/// @param _currentState 現在のステート
	/// @return 変更したか？
	static bool ImGuiComboPlayerState(const std::string& _caption, PlayerState& _currentState);

private:
	/// @brief どのアクションにも共通する遷移
	void CommmonCheckTransition();

	/// @brief キャンセルに合わせてアクションが行うか確認
	/// @return ステートをここで変更したか？
	bool CheckCanCancelTransition();

	bool CheckCanRolling();

protected:
	// アニメーションコントローラ内のプレイヤー名
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// 移動速度
	constexpr static auto MOVEAXIS_X_PARAMNAME = "axisX";	// 移動速度
	constexpr static auto MOVEAXIS_Y_PARAMNAME = "axisY";	// 攻撃トリガー
};

