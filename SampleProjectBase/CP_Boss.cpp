#include "pch.h"
#include "CP_Boss.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

CP_Boss::CP_Boss()
	: pAnimation(nullptr)
{
}

void CP_Boss::Init()
{
	CP_Enemy::Init();

	pActionController = std::make_unique<BossActionController>(*this);
}

void CP_Boss::SetAttackInfo(const HashiTaku::AttackInformation& _attackInfo)
{
	pWeapon->SetAttackInfo(_attackInfo);
}

nlohmann::json CP_Boss::Save()
{
	auto data = CP_Enemy::Save();

	data["actionController"] = pActionController->Save();

	return data;
}

void CP_Boss::Load(const nlohmann::json& _data)
{
	CP_Enemy::Load(_data);

	nlohmann::json actionControllerData;
	if (HashiTaku::LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);
}

void CP_Boss::Awake()
{
	CP_Enemy::Awake();
}

void CP_Boss::Start()
{
	CP_Enemy::Start();

	pAnimation = GetGameObject().GetComponent<CP_Animation>();
	CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

	pActionController->Init(pAnimation, pRb);

	// プレイヤーを取得する
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pActionController->SetPlayer(*pBattle->GetPlayerObject());
	}
}

void CP_Boss::Update()
{
	CP_Enemy::Update();

	pActionController->UpdateCall();
}

void CP_Boss::Draw()
{
	CP_Enemy::Draw();

	pActionController->DebugDisplay();
}

void CP_Boss::SetupWeapon()
{
	auto pWeaponObj = InSceneSystemManager::GetInstance()->
		GetSceneObjects().GetSceneObject(weaponObjName);
	if (!pWeaponObj) return;

	// 武器コンポーネントを取得
	if (CP_Weapon* pGetWeapon = pWeaponObj->GetComponent<CP_Weapon>())
	{
		pWeapon = pGetWeapon;
	}
}

bool CP_Boss::GetCanUpdate()
{
	return true;
}

void CP_Boss::OnHitStopBegin()
{
	CP_Enemy::OnHitStopBegin();
}

void CP_Boss::OnHitStopEnd()
{
	CP_Enemy::OnHitStopEnd();
}

void CP_Boss::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo)
{
	CP_Enemy::OnDamageBehavior(_attackInfo);

	pActionController->OnDamage(_attackInfo);
}

void CP_Boss::ImGuiDebug()
{
	CP_Enemy::ImGuiDebug();
	ImGui::InputText("weaponName", &weaponObjName[0], IM_INPUT_BUF);
	pActionController->ImGuiCall();
}
