#pragma once
#include "PlayerActionController_Base.h"
#include "PlayerAirState.h"

template<typename T>
concept PlayerAirConcept =
std::is_base_of_v<PlayerAirState, T>;


class PlayerAirActionController :
	public PlayerActionController_Base
{
	using AirState = PlayerAirState::PlayerState;
public:
	/// @brief コンストラクタ
	/// @param _pAction アクション
	/// @param _player プレイヤー
	PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player);
	~PlayerAirActionController() {}

	/// @brief 行動状態を切り替える
	/// @param _nextActionName 次の状態の名前
	/// @param _isForce 強制で切り替えするか？
	bool ChangeAirState(const  AirState& _nextActionState, bool _isForce = false);

private:
	template<PlayerAirConcept T>
	void CreateState(AirState _actionState);
	
};

template<PlayerAirConcept T>
inline void PlayerAirActionController::CreateState(AirState _actionState)
{
	PlayerActionController_Base::CreateState<T>(static_cast<int>(_actionState));
}