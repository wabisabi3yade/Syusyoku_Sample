#pragma once

#include "PlayerState_Base.h"

/// @brief プレイヤーの動きコントローラー
class PlayerActionController
{
public:
	enum class State
	{
		Move,
		Attack,
		MaxNum
	};

private:
	/// @brief プレイヤーの行動状態リスト
	std::unordered_map<State, std::unique_ptr<PlayerState_Base>> stateList;

	/// @brief 現在の行動クラス
	PlayerState_Base* pCurrentState;

	/// @brief プレイヤーのオブジェクト
	GameObject* pPlayerObject;

	// 現在の状態
	State nowState;
public:
	PlayerActionController(GameObject& _pPlayerObject);
	~PlayerActionController() {}

	/// @brief 現在の更新処理
	void Update();

	/// @brief 行動状態を切り替える
	/// @param _nextState 次の状態
	void TransitionState(State _nextState);

	void ImGuiSetting();
};

