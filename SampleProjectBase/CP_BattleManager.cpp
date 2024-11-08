#include "pch.h"
#include "CP_BattleManager.h"
#include "GameObject.h"

void CP_BattleManager::SetPlayer(GameObject& _playerObj)
{
	pPlayerObject = &_playerObj;
}

void CP_BattleManager::AddEnemy(GameObject& _enemyObj)
{
	auto enemyItr = std::find(enemyList.begin(), enemyList.end(), &_enemyObj);
	
	if (enemyItr != enemyList.end())
	{
		HASHI_DEBUG_LOG(_enemyObj.GetName() + "‚ÍŠù‚É’Ç‰Á‚³‚ê‚Ä‚¢‚Ü‚·");
		return;
	}

	enemyList.push_back(&_enemyObj);
}

void CP_BattleManager::RemovePlayer(GameObject& _playerObj)
{
	if (pPlayerObject != &_playerObj) return;

	pPlayerObject = nullptr;
}

void CP_BattleManager::RemoveEnemy(GameObject& _enemyObj)
{
#ifdef EDIT
	auto enemyItr = std::find(enemyList.begin(), enemyList.end(), &_enemyObj);
	if (enemyItr == enemyList.end())
	{
		HASHI_DEBUG_LOG(_enemyObj.GetName() + "‚ªƒŠƒXƒg“à‚É‚ ‚è‚Ü‚¹‚ñ");
		return;
	}
#endif // EDIT

	enemyList.remove(&_enemyObj);
}

GameObject* CP_BattleManager::GetPlayerObject()
{
	return pPlayerObject;
}
