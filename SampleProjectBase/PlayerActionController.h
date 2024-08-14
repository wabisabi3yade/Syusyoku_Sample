#pragma once

#include "PlayerActState_Base.h"

class PlayerAnimController;

/// @brief プレイヤーの動きコントローラー
class PlayerActionController
{
public:
	// プレイヤーの行動状態
	enum class State
	{
		Move,
		Jump,
		Attack,
		MaxNum
	};

private:
	/// @brief プレイヤーの行動状態リスト
	std::unordered_map<State, std::unique_ptr<PlayerActState_Base>> pActions;

	/// @brief 現在の行動クラス
	PlayerActState_Base* pCurrentState;

	/// @brief アニメーションのコントローラー
	PlayerAnimController* pAnimController;

	/// @brief プレイヤーのオブジェクト
	GameObject* pPlayerObject;

	// 現在の状態
	State nowState;
public:
	PlayerActionController(GameObject& _pPlayerObject, PlayerAnimController& _animController);
	~PlayerActionController() {}

	/// @brief 現在の更新処理
	void Update();

	/// @brief 行動状態を切り替える
	/// @param _nextState 次の状態
	void TransitionState(State _nextState);

	///// @brief アニメーションを遷移させる
	///// @param _nextAnimation 次のアニメーションの名前
	//void TransitionAnimation(const std::string& _nextName);

	void ImGuiSetting();

private:
	/// @brief 新しくStateを生成
	/// @tparam T 対応している行動クラス
	/// @param _state 生成する状態
	template <class T> void CreateState(State _state);
};

template<class T>
inline void PlayerActionController::CreateState(State _state)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>(*this);

	createState->SetPlayerObject(*pPlayerObject);
	createState->SetAnimController(*pAnimController);

	pActions[_state] = std::move(createState);
}
