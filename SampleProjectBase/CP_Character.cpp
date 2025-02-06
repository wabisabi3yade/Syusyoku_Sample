#include "pch.h"
#include "CP_Character.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{

	// �q�b�g�X�g�b�v���ɐi�߂鑬�x
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

		// �e�h���̃_���[�W����
		bool isDamage = OnDamageBehavior(_attackInfo);

		// �_���[�W���󂯂Ă��Ȃ��Ȃ�
		if (!isDamage) return;

		// �_���[�W�󂯂��Ƃ��̏���
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

		// �I�u�W�F�N�g�̌o�ߑ��x���~�߂�
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
		// �q�b�g�X�g�b�v�}�l�[�W���[�Ɏ��g����
		AddToHitStopManager();

		// �ő�̗͂ɍ��킹��
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
		// �ő�l�𒴂��Ȃ��悤��
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
		// �̗͂����炷
		DecadeHp(_atkInfo.GetDamageValue());

		// �̗͂��Ȃ��Ȃ�����
		if (currentHP <= 0.0f)
			OnDeath();

		// �G�t�F�N�g
		CreateVfx(_atkInfo.GetHitVfxInfo(), _contactWorldPos);

		// �T�E���h
		CreateSoundFX(_atkInfo.GetHitSEParam(), _contactWorldPos);
	}

	void CP_Character::CreateVfx(const CreateVfxInfo& _vfxInfo,
		const DXSimp::Vector3& _createPos)
	{
		// �Đ�
		DX11EffecseerManager::GetInstance()->Play(_vfxInfo, _createPos);
	}

	void CP_Character::CreateSoundFX(const PlaySoundParameter& _soundParam,
		const DXSimp::Vector3& _soundPos)
	{
		CP_SoundManager* pSound = CP_SoundManager::GetInstance();
		if (!pSound) return;

		// �Փ˒n�_�ɍĐ�����
		pSound->PlaySE(_soundParam, _soundPos);
	}
}