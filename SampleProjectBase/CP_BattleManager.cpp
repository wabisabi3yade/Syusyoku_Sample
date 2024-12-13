#include "pch.h"
#include "CP_BattleManager.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"
#include "Geometory.h"
namespace DXSimp = DirectX::SimpleMath;

void CP_BattleManager::SetPlayer(CP_Player& _playerObj)
{
	pPlayer = &_playerObj;
}

void CP_BattleManager::AddEnemy(CP_Enemy& _enemyObj)
{
	auto itr = std::find(enemyList.begin(), enemyList.end(), &_enemyObj);

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

void CP_BattleManager::OnPlayerWin()
{
	// ほぼ同タイミングで起きた場合に重複しないようにする
	if (curBattleState == BattleState::Lose) return;

	curBattleState = BattleState::Win;
	HASHI_DEBUG_LOG("プレイヤー勝ち");
}

void CP_BattleManager::OnPlayerLose()
{
	// ほぼ同タイミングで起きた場合に重複しないようにする
	if (curBattleState == BattleState::Win) return;

	curBattleState = BattleState::Lose;
	HASHI_DEBUG_LOG("プレイヤー負け");
}

nlohmann::json CP_BattleManager::Save()
{
	auto data = SingletonComponent::Save();

	HashiTaku::SaveJsonVector4("moveAreaRect", moveAreaRect, data);

	return data;
}

void CP_BattleManager::Load(const nlohmann::json& _data)
{
	SingletonComponent::Load(_data);

	HashiTaku::LoadJsonVector4("moveAreaRect", moveAreaRect, _data);
}

void CP_BattleManager::LateUpdate()
{
	// 移動範囲制限
	MoveAreaUpdate();
}

void CP_BattleManager::Draw()
{
#ifdef EDIT
	if (!isDebugDisplay) return;

	constexpr float height = 0.3f;
	DXSimp::Color c = DXSimp::Color(1, 0, 0);

	DXSimp::Vector3 RT = { moveAreaRect.x, height, moveAreaRect.z };
	DXSimp::Vector3 RB = { moveAreaRect.x, height, moveAreaRect.w };
	DXSimp::Vector3 LT = { moveAreaRect.y, height, moveAreaRect.z };
	DXSimp::Vector3 LB = { moveAreaRect.y, height, moveAreaRect.w };

	Geometory::AddLine(RT, RB, c);
	Geometory::AddLine(RB, LB, c);
	Geometory::AddLine(LB, LT, c);
	Geometory::AddLine(LT, RT, c);
#endif // EDIT
}

void CP_BattleManager::MoveAreaUpdate()
{
	// 移動範囲内に制限

	if (pPlayer)
		PositionClamp(pPlayer->GetTransform());

	for (auto& enemy : enemyList)
	{
		PositionClamp(enemy->GetTransform());
	}
}

void CP_BattleManager::PositionClamp(Transform& _charaTransform)
{
	//　y座標を考慮しない距離を求める
	DXSimp::Vector3 pos = _charaTransform.GetPosition();
	
	pos.x = std::clamp(pos.x, moveAreaRect.y, moveAreaRect.x);
	pos.z = std::clamp(pos.z, moveAreaRect.w, moveAreaRect.z);

	_charaTransform.SetPosition(pos);
}

void CP_BattleManager::ImGuiDebug()
{
#ifdef EDIT
	ImGui::Checkbox("IsDisplay", &isDebugDisplay);
	ImGui::DragFloat4("AreaRect", &moveAreaRect.x, 0.01f);
#endif // EDIT
}
