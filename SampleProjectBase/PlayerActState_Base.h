#pragma once

// ゲーム内入力
#include "GameInput.h"
#include "IObserever.h"
#include "CP_Camera.h"

#include "AnimationController.h"

class CP_Animation;
class GameObject;

/// @brief プレイヤーの行動ステート基底クラス
class PlayerActState_Base : public HashiTaku::IImGuiUser, HashiTaku::ISaveLoad
{
public:
	// プレイヤーの行動状態
	enum class StateType
	{
		Idle,
		Move,
		TargetMove,
		Jump,
		Attack,
		MaxNum
	};

private:
	/// @brief ステート遷移することを通知をするサブジェクト
	std::unique_ptr<HashiTaku::Subject<int>> changeStateSubject;

	/// @brief この行動クラスのステートタイプ
	StateType stateType;
protected:
	/// @brief アニメーション管理
	CP_Animation* pAnimation;

	/// @brief  プレイヤーオブジェクト
	GameObject* pPlayerObject;

	GameInput* pPlayerInput;

	static CP_Camera* pCamera;
public:
	PlayerActState_Base(StateType _stateType);
	virtual ~PlayerActState_Base() {}

	/// @brief 初期化処理
	/// @param _gameObject　移動するオブジェクト 
	/// @param _changeObserver ステート遷移オブザーバー
	void Init(GameObject& _gameObject, HashiTaku::IObserver<int>& _changeObserver);

	/// @brief 開始処理呼び出し
	void OnStartCall();

	/// @brief 更新処理呼び出し
	void UpdateCall();

	/// @brief  終了処理呼び出し
	void OnEndCall();

	// アニメーションをセットする
	void SetAnimation(CP_Animation& _pAnimation);

	/// @brief アクション状態列挙型を文字列に変換
	/// @param _stateType 文字列に変換したいアクション状態
	/// @return 文字列
	static std::string StateTypeToStr(StateType _stateType);

	// ステートタイプを取得
	StateType GetActStateType() const;

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief 各状態の開始処理
	virtual void OnStart() = 0;

	/// @brief 更新処理
	virtual void Update() = 0;

	/// @brief 各状態の終了処理
	virtual void OnEnd() = 0;

	/// @brief 状態を遷移する
	/// @param _changeSate 遷移先の状態
	void ChangeState(StateType _changeState);

	/// @brief	入力値を返す
	DirectX::SimpleMath::Vector2 InputValue();

	/// @brief ImGui処理
	virtual void ImGuiSetting() {}

protected:
	// アニメーションコントローラ内のプレイヤー名
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// 移動速度
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// 攻撃トリガー
	constexpr static auto MOVEAXIS_X_PARAMNAME = "AxisX";	// 移動速度
	constexpr static auto MOVEAXIS_Y_PARAMNAME = "AxisY";	// 攻撃トリガー
};

