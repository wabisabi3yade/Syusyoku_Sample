#pragma once

// ゲーム内入力
#include "GameInput.h"

class PlayerActionController;
class PlayerAnimController;
class GameObject;

/// @brief プレイヤーの行動State基底クラス
class PlayerActState_Base
{
protected:
	// アクション管理
	PlayerActionController* pActionController;

	// アニメーション管理
	PlayerAnimController* pAnimController;

	/// @brief  プレイヤーオブジェクト
	GameObject* pPlayerObject;

protected:
	/// @brief 更新処理
	virtual void Update() = 0;

	/// @brief 遷移条件を確認する
	virtual void TransitionCheck() = 0;

	/// @brief ボタンが押されたら次の状態に遷移させる
	/// @param _buttonType ボタンの種類
	/// @param _nextState 次の状態
	void ButtonChangeState(GameInput::ButtonType _buttonType, 
		u_int _nextState);

	/// @brief アニメーションを変更
	/// @param _animState アニメーションをのステート
	void ChangeAnimation(u_int _animState);
public:
	PlayerActState_Base(PlayerActionController& _pController);
	~PlayerActState_Base() {}

	/// @brief 状態が変わったときの初期処理
	virtual void Init() = 0;

	/// @brief 外部から呼び出す用更新処理
	void UpdateBase();

	/// @brief  終了処理
	virtual void Terminal() = 0;

	/// @brief ImGui処理
	virtual void ImGuiSetting() {}

	// プレイヤーオブジェクトをセット
	void SetPlayerObject(GameObject& _playerObj);

	// アニメーション管理をセットする
	void SetAnimController(PlayerAnimController& _pController);
};

