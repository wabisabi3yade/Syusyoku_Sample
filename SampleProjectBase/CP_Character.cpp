#include "pch.h"
#include "CP_Character.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{

	// ヒットストップ中に進める速度
	constexpr float TIMESPEED_ON_HITSTOP(0.015f);
	
	CP_Character::CP_Character() :
		currentHP(0.0f),
		maxHP(1.0f),
		hsBeforeDeltaTime(0.0f),
		isDead(false),
		isHitStopping(false),
		isInvicible(false)
	{
	}

	void CP_Character::OnDestroy()
	{
	}

	void CP_Character::SetMaxHP(float _hitPoint)
	{
		maxHP = std::clamp(_hitPoint, 0.0f, MAXLIMIT_HP);
	}

	void CP_Character::SetIsInvicible(bool _isInvicible)
	{
		isInvicible = _isInvicible;
	}

	float CP_Character::GetCurrentHP() const
	{
		return currentHP;
	}

	bool CP_Character::GetIsInvicible() const
	{
		return isInvicible;
	}

	bool CP_Character::GetDead() const
	{
		return isDead;
	}

	void CP_Character::OnDamage(AttackInformation& _attackInfo,
		const DXSimp::Vector3& _contactPos)
	{
		if (isInvicible) return;

		// 各派生のダメージ処理
		bool isDamage = OnDamageBehavior(_attackInfo);

		// ダメージを受けていないなら
		if (!isDamage) return;

		// ダメージ受けたときの処理
		OnTakeDamage(_attackInfo, _contactPos);
	}

	void CP_Character::OnDeath()
	{
		isDead = true;

		OnDeathBehavior();
	}

	void CP_Character::OnHitStopBegin()
	{
		isHitStopping = true;

		// オブジェクトの経過速度を止める
		GameObject& go = GetGameObject();
		hsBeforeDeltaTime = go.GetDeltaSpeed();
		go.SetDeltaTimeSpeed(TIMESPEED_ON_HITSTOP);
	}

	void CP_Character::OnHitStopEnd()
	{
		isHitStopping = false;

		GameObject& go = GetGameObject();
		go.SetDeltaTimeSpeed(hsBeforeDeltaTime);
		hsBeforeDeltaTime = 0.0f;
	}

	json CP_Character::Save()
	{
		auto data = Component::Save();
		data["maxHp"] = maxHP;
		return data;
	}

	void CP_Character::Load(const json& _data)
	{
		Component::Load(_data);
		LoadJsonFloat("maxHp", maxHP, _data);
	}

	bool CP_Character::GetIsHitStopping() const
	{
		return isHitStopping;
	}

	void CP_Character::Start()
	{
		// ヒットストップマネージャーに自身を代入
		AddToHitStopManager();

		// 最大体力に合わせる
		currentHP = maxHP;
	}

	void  CP_Character::BeginHitStop(u_int _hitStopFlame)
	{
		if (CP_HitStopManager* pHitStopManager = CP_HitStopManager::GetInstance())
		{
			pHitStopManager->HitStopBegin(_hitStopFlame);
		}
	}

	void CP_Character::SetCurrentHP(float _setHp)
	{
		// 最大値を超えないように
		currentHP = std::min(_setHp, maxHP);
	}

	void CP_Character::DecadeHp(float _damageVal)
	{
		float setHp = currentHP - _damageVal;
		SetCurrentHP(setHp);
	}

	void  CP_Character::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Checkbox("IsInvicible", &isInvicible);
		if (ImGui::DragFloat("CurrentHP", &currentHP, 0.1f, 0.0f, MAXLIMIT_HP))
		{
			SetCurrentHP(currentHP);
		}
		ImGui::DragFloat("MaxHP", &maxHP, 0.1f, 0.0f, MAXLIMIT_HP);
#endif // EDIT
	}

	void CP_Character::OnTakeDamage(const AttackInformation& _atkInfo,
		const DXSimp::Vector3& _contactWorldPos)
	{
		// 体力を減らす
		DecadeHp(_atkInfo.GetDamageValue());

		// 体力がなくなったら
		if (currentHP <= 0.0f)
			OnDeath();

		// エフェクト
		CreateVfx(_atkInfo.GetHitVfxInfo(), _contactWorldPos);

		// サウンド
		CreateSoundFX(_atkInfo.GetHitSEParam(), _contactWorldPos);
	}

	void CP_Character::CreateVfx(const CreateVfxInfo& _vfxInfo,
		const DXSimp::Vector3& _createPos)
	{
		// 再生
		DX11EffecseerManager::GetInstance()->Play(_vfxInfo, _createPos);
	}

	void CP_Character::CreateSoundFX(const PlaySoundParameter& _soundParam,
		const DXSimp::Vector3& _soundPos)
	{
		CP_SoundManager* pSound = CP_SoundManager::GetInstance();
		if (!pSound) return;

		// 衝突地点に再生する
		pSound->PlaySE(_soundParam, _soundPos);
	}
}