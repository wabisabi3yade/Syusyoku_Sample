#include "pch.h"
#include "CP_Boss.h"
#include "GameObject.h"

CP_Boss::CP_Boss()
	: pAnimation(nullptr), hitStopBeforeAnimSpeed(0.0f)
{
}

void CP_Boss::Init()
{
	CP_Enemy::Init();

	pActionController = std::make_unique<BossActionController>(*this);
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

	if (pAnimation = GetGameObject().GetComponent<CP_Animation>())
	{
		pActionController->Init(*pAnimation);
	}

	// プレイヤーを取得する
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pActionController->SetPlayer(*pBattle->GetPlayerObject());
	}
}

void CP_Boss::Update()
{
	if (!GetCanUpdate()) return;

	CP_Enemy::Update();

	pActionController->Update();
}

void CP_Boss::Draw()
{
	CP_Enemy::Draw();

	pActionController->DebugDisplay();
}

bool CP_Boss::GetCanUpdate()
{
	if (GetIsHitStopping()) return false;

	return true;
}

void CP_Boss::OnHitStopBegin()
{
	CP_Enemy::OnHitStopBegin();

	// アニメーションを止める
	hitStopBeforeAnimSpeed = pAnimation->GetControllerPlaySpeed();
	pAnimation->SetControllerPlaySpeed(0.0f);
}

void CP_Boss::OnHitStopEnd()
{
	CP_Enemy::OnHitStopEnd();

	// アニメーションの速度を戻す
	pAnimation->SetControllerPlaySpeed(hitStopBeforeAnimSpeed);
}

void CP_Boss::ImGuiSetting()
{
	CP_Enemy::ImGuiSetting();

	pActionController->ImGuiCall();
}
