#include "pch.h"
#include "PlayerAirActionController.h"

PlayerAirActionController::PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player) :
	PlayerActionController_Base(_pAction, _player, "playerAirController")
{
	place = ActionPlace::Air;

	// �s���N���X�𐶐�
	using enum AirState;

	// �f�t�H���g��Ԃ��Z�b�g
	SetDefaultNode(static_cast<int>(Idle));
}

bool PlayerAirActionController::ChangeAirState(const AirState& _nextActionState, bool _isForce)
{
	return ChangeState(static_cast<int>(_nextActionState), _isForce);
}
