#pragma once

class PlayerActionController;

class GameObject;

class PlayerState_Base
{
protected:
	PlayerActionController* pController;

	/// @brief  プレイヤーのトランスフォーム
	GameObject* pGameObject;

public:
	PlayerState_Base(PlayerActionController& _pController, GameObject& _gameObject) : pController(&_pController), pGameObject(&_gameObject) {}
	~PlayerState_Base() {}

	/// @brief 状態が変わったときの初期処理
	virtual void Init() = 0;

	/// @brief 更新処理
	virtual void Update() = 0;

	/// @brief  終了処理
	virtual void Terminal() = 0;
};

