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
	/// @brief �R���X�g���N�^
	/// @param _pAction �A�N�V����
	/// @param _player �v���C���[
	PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player);
	~PlayerAirActionController() {}

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextActionName ���̏�Ԃ̖��O
	/// @param _isForce �����Ő؂�ւ����邩�H
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