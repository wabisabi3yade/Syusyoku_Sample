#include "pch.h"
#include "GameMode.h"

GameObject* GameMode::GetPlayer(u_int _idx)
{
	if (_idx >= static_cast<u_int>(pPlayers.size() || _idx < 0)) return nullptr;

	return pPlayers[_idx];
}

void GameMode::SetCamera(CP_Camera& _camera)
{
	pMainCamera = &_camera;
}

void GameMode::SetPleyer(GameObject& _player, u_int _idx)
{
	if (_idx < 0) return;

	pPlayers[_idx] = &_player;
}
