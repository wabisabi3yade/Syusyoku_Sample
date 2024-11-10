#include "pch.h"
#include "CP_BattleManager.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

void CP_BattleManager::SetPlayer(CP_Player& _playerObj)
{
	pPlayer = &_playerObj;
}

void CP_BattleManager::AddEnemy(CP_Enemy& _enemyObj)
{
	auto itr =  std::find(enemyList.begin(), enemyList.end(), &_enemyObj);

	if (itr != enemyList.end()) return;

	enemyList.push_back(&_enemyObj);
}
void CP_BattleManager::RemovePlayer(CP_Player& _playerObj)
{
	if (pPlayer != &_playerObj) return;

	pPlayer = nullptr;
}

void CP_BattleManager::RemoveEnemy(CP_Enemy& _enemyObj)
{
	enemyList.remove(&_enemyObj);
}


CP_Player* CP_BattleManager::GetPlayerObject()
{
	return pPlayer;
}

const CP_BattleManager::EnemyList& CP_BattleManager::GetEnemyList()
{
	return enemyList;
}

void CP_BattleManager::ImGuiSetting()
{
	static char input[IM_INPUT_BUF];

	ImGui::InputText("object", input, IM_INPUT_BUF);
}
