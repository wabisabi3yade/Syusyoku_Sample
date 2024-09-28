#pragma once
#include "PlayerActState_Base.h"
#include "PlayerActChangeObserver.h"

class AnimationController;

/// @brief プレイヤーの動きコントローラー
class PlayerActionController
{
private:
	/// @brief プレイヤーの行動状態リスト
	std::unordered_map<PlayerActState_Base::StateType, std::unique_ptr<PlayerActState_Base>> pActions;

	/// @brief ステートから変更した時に通知を受け取るオブザーバー
	std::unique_ptr<PlayerActChangeObserver> pStateChangeObserver;

	/// @brief 現在の行動クラス
	PlayerActState_Base* pCurrentState;

	/// @brief アニメーションのコントローラー
	AnimationController* pAnimController;

	/// @brief プレイヤーのオブジェクト
	GameObject* pPlayerObject;
public:
	PlayerActionController(GameObject& _pPlayerObject);
	~PlayerActionController() {}

	/// @brief 開始処理
	/// @param _animationController アニメーションコントローラー
	void Begin(AnimationController& _animationController);

	/// @brief 現在の更新処理
	void Update();

	/// @brief 行動状態を切り替える
	/// @param _nextState 次の状態
	void ChangeState(PlayerActState_Base::StateType _nextState);

	/// @brief 初期の状態をセット
	/// @param _defaultState 初期状態のステート
	void DefaultState(PlayerActState_Base::StateType _defaultState);

	void ImGuiSetting();

private:
	/// @brief 新しくStateを生成
	/// @tparam T 対応している行動クラス
	/// @param _state 生成する状態
	template <class T> void CreateState(PlayerActState_Base::StateType _state);
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::StateType _state)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(*pPlayerObject, *pStateChangeObserver);

	pActions[_state] = std::move(createState);
}
