#include "pch.h"
#include "CP_Character.h"

HashiTaku::CP_Character::CP_Character()
	: currentHP(0.0f), maxHP(1.0f), isDead(false), isHitStopping(false)
{
}

void HashiTaku::CP_Character::SetMaxHP(float _hitPoint)
{	
	maxHP = std::clamp(_hitPoint, 0.0f, MAXLIMIT_HP);
}

float HashiTaku::CP_Character::GetCurrentHP() const
{
	return currentHP;
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

void HashiTaku::CP_Character::ImGuiSetting()
{
	ImGui::DragFloat("CurrentHP", &currentHP, 0.1f, 0.0f, MAXLIMIT_HP);
	ImGui::DragFloat("MaxHP", &maxHP, 0.1f, 0.0f, MAXLIMIT_HP);
}
