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

nlohmann::json CP_BattleManager::Save()
{
	auto data = SingletonComponent::Save();

	HashiTaku::LoadJsonVector3("centerPos", moveAreaCenterPos, data);
	data["radius"] = moveAreaRadius;

	return data;
}

void CP_BattleManager::Load(const nlohmann::json& _data)
{
	SingletonComponent::Load(_data);

	HashiTaku::LoadJsonVector3("centerPos", moveAreaCenterPos, _data);
	HashiTaku::LoadJsonFloat("radius", moveAreaRadius, _data);
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
	
	Geometory::SetPosition(moveAreaCenterPos);
	Geometory::SetScale(DXSimp::Vector3::One * moveAreaRadius * 2);
	Geometory::SetColor(DXSimp::Color(1, 1, 1, 0.2f));
	Geometory::DrawSphere();
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
	DXSimp::Vector3 distance = _charaTransform.GetPosition() - moveAreaCenterPos;
	distance.y = 0.0f;
	float disLength = distance.Length();

	// 範囲から超えていたら
	if (disLength > moveAreaRadius)	
	{
		// 単位ベクトルを求める
		DXSimp::Vector3 vec;
		distance.Normalize(vec);

		// 範囲内に移動させる
		float posY = _charaTransform.GetPosition().y;
		DXSimp::Vector3 clampPos = moveAreaCenterPos + vec * moveAreaRadius;
		clampPos.y = posY;
		_charaTransform.SetPosition(clampPos);
	}
}

void CP_BattleManager::ImGuiDebug()
{
	ImGui::Checkbox("IsDisplay", &isDebugDisplay);
	ImGui::DragFloat("AreaRadius", &moveAreaRadius, 0.1f, 0.0f, 10000.0f);
	ImGuiMethod::DragFloat3(moveAreaCenterPos, "AreaCenter", 0.1f);
}
