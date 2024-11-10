#include "pch.h"
#include "EnemyActionController.h"
#include "CP_Enemy.h"

EnemyActionController::EnemyActionController(CP_Enemy& _enemy, const std::string& _controllerName)
	: CharacterActionController(_enemy, _controllerName)
{
}
