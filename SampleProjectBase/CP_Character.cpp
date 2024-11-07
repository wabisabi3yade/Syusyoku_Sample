#include "pch.h"
#include "CP_Character.h"
#include "CP_HitStopManager.h"

HashiTaku::CP_Character::CP_Character()
	: currentHP(0.0f), maxHP(1.0f), isDead(false), isHitStopping(false), isInvicible(false)
{
}

void HashiTaku::CP_Character::SetMaxHP(float _hitPoint)
{	
	maxHP = std::clamp(_hitPoint, 0.0f, MAXLIMIT_HP);
}

void HashiTaku::CP_Character::SetIsInvicible(bool _isInvicible)
{
	isInvicible = _isInvicible;
}

float HashiTaku::CP_Character::GetCurrentHP() const
{
	return currentHP;
}

bool HashiTaku::CP_Character::GetIsInvicible() const
{
	return isInvicible;
}

void HashiTaku::CP_Character::OnDamage(const AttackInformation& _attackInfo)
{
	OnDamageBehavior(_attackInfo);

	// 体力がなくなったら
	if (currentHP <= 0.0f)
		OnDeath();
}

void HashiTaku::CP_Character::OnDeath()
{
	isDead = true;

	OnDeathBehavior();
}

void HashiTaku::CP_Character::OnHitStopBegin()
{
	isHitStopping = true;
}

void HashiTaku::CP_Character::OnHitStopEnd()
{
	isHitStopping = false;
}

nlohmann::json HashiTaku::CP_Character::Save()
{
	auto data = Component::Save();
	data["maxHp"] = maxHP;
	return data;
}

void HashiTaku::CP_Character::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadJsonFloat("maxHp", maxHP, _data);
}

bool HashiTaku::CP_Character::GetIsHitStopping() const
{
	return isHitStopping;
}

void HashiTaku::CP_Character::Start()
{
	// ヒットストップマネージャーに自身を代入
	AddToHitStopManager();

	// 最大体力に合わせる
	currentHP = maxHP;
}

void HashiTaku::CP_Character::BeginHitStop(u_int _hitStopFlame)
{
	if (CP_HitStopManager* pHitStopManager = CP_HitStopManager::GetInstance())
	{
		pHitStopManager->HitStopBegin(_hitStopFlame);
	}
}

void HashiTaku::CP_Character::ImGuiSetting()
{
	ImGui::Checkbox("IsInvicible", &isInvicible);
	ImGui::DragFloat("CurrentHP", &currentHP, 0.1f, 0.0f, MAXLIMIT_HP);
	ImGui::DragFloat("MaxHP", &maxHP, 0.1f, 0.0f, MAXLIMIT_HP);
}
