#include "pch.h"
#include "CP_Boss.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"
#include "PlayerAttackInformation.h"

namespace HashiTaku
{
	constexpr auto CAN_ATTACK_ANIMPARAM("attackCollision");	// �U���\�t���O�̃A�j���p������
	constexpr auto DEAD_ANIMPARAM("deadTrigger");	// ���񂾂Ƃ��̃A�j���p������
	constexpr auto BREAK_ANIMPARAM("isBreaking");	// �u���C�N��Ԃ̃A�j���p������

	CP_Boss::CP_Boss() :
		pAnimation(nullptr),
		pWeapon(nullptr),
		pHpBar(nullptr),
		pBreakBar(nullptr),
		pCanAttack(nullptr),
		curBreakValue(0.0f),
		decadeBreakSpeed(10.0f),
		maxBreakValue(100.0f),
		isBreaking(false)
	{
	}

	void CP_Boss::Init()
	{
		CP_Enemy::Init();

		pActionController = std::make_unique<BossActionController>(*this);
	}

	void CP_Boss::SetAttackInfo(const AttackInformation& _attackInfo)
	{
		if (!pWeapon) return;

		pWeapon->SetAttackInfo(_attackInfo);
	}

	bool CP_Boss::GetIsBreaking() const
	{
		return isBreaking;
	}

	json CP_Boss::Save()
	{
		auto data = CP_Enemy::Save();

		data["actionController"] = pActionController->Save();
		data["weaponName"] = weaponObjName;
		data["hpBarName"] = hpBarObjName;
		data["breakBarName"] = breakBarObjName;

		return data;
	}

	void CP_Boss::Load(const json& _data)
	{
		CP_Enemy::Load(_data);

		LoadJsonString("weaponName", weaponObjName, _data);
		LoadJsonString("hpBarName", hpBarObjName, _data);
		LoadJsonString("breakBarName", breakBarObjName, _data);

		json actionControllerData;
		if (LoadJsonData("actionController", actionControllerData, _data))
			pActionController->Load(actionControllerData);
	}

	void CP_Boss::Awake()
	{
		CP_Enemy::Awake();
	}

	void CP_Boss::Start()
	{
		CP_Enemy::Start();

		// �I�u�W�F�N�g���擾
		FindRequaireObject();

		// �R���|�[�l���g�擾
		pAnimation = GetGameObject().GetComponent<CP_Animation>();
		CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

		// �v���C���[���擾����
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pActionController->SetPlayer(*pBattle->GetPlayerObject());
		}

		// �A�j���[�V�����p�����[�^�̃A�h���X���擾
		pCanAttack = pAnimation->GetParameterPointer<bool>(CAN_ATTACK_ANIMPARAM);

		// �A�N�V�����R���g���[���[������s
		pActionController->Init(pAnimation, pRb);
	}

	void CP_Boss::Update()
	{
		if (!CanUpdate()) return;

		CP_Enemy::Update();

		pActionController->UpdateCall();

		// �u���C�N�X�V
		BreakUpdate();

		// �U��������X�V
		UpdateAttackCollision();
	}

	void CP_Boss::Draw()
	{
		CP_Enemy::Draw();

#ifdef EDIT
		// �A�N�V�������ŕ\�����������f�o�b�O�`��
		pActionController->DebugDisplay();
#endif // EDIT
	}

	bool CP_Boss::CanUpdate()
	{
		if (GetDead()) return false;

		return true;
	}

	void CP_Boss::UpdateAttackCollision()
	{
#ifdef EDIT
		if (!pWeapon || !pCanAttack) return;
#endif // EDIT

		// ����̍U��������Z�b�g����
		pWeapon->SetIsAttackCollision(*pCanAttack);
	}

	void CP_Boss::BreakUpdate()
	{
		if (!isBreaking) return;

		// ���炵�Ă���
		curBreakValue -= decadeBreakSpeed * DeltaTime();

		// 0�ɂȂ�����
		if (curBreakValue < 0.0f)
		{
			curBreakValue = 0.0f;
			OnBreakEnd();
		}

		// �X���C�_�[�ɔ��f
		pBreakBar->SetCurrentValue(curBreakValue);
	}

	void CP_Boss::FindRequaireObject()
	{
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

		// ����R���|�[�l���g���擾
		GameObject* pFindObj = sceneObjs.GetSceneObject(weaponObjName);
		if (!pFindObj) return;
		if (pWeapon = pFindObj->GetComponent<CP_Weapon>())
		{
			// �����̍��W��n��
			pWeapon->SetHaveObjPosPointer(&GetTransform().GetPosition());
		}

		// �̗̓o�[
		if (pFindObj = sceneObjs.GetSceneObject(hpBarObjName))
		{
			pHpBar = pFindObj->GetComponent<IUISlider>();
			if (pHpBar)
			{
				pHpBar->SetMaxValue(maxHP);
				pHpBar->SetCurrentValue(currentHP);
			}
		}

		// �u���C�N�o�[
		if (pFindObj = sceneObjs.GetSceneObject(breakBarObjName))
		{
			pBreakBar = pFindObj->GetComponent<IUISlider>();
			if (pBreakBar)
			{
				pBreakBar->SetMaxValue(maxBreakValue);
				pBreakBar->SetCurrentValue(curBreakValue);
			}
		}
	}

	void CP_Boss::SetCurrentHP(float _setHp)
	{
		CP_Character::SetCurrentHP(_setHp);

		// �̗̓o�[�ɂ����f
		if (pHpBar)
			pHpBar->SetCurrentValue(currentHP);
	}

	void CP_Boss::AddBreakValue(float _add)
	{
		// �u���C�N���͉��Z���Ȃ�
		if (isBreaking) return;

		curBreakValue += _add;
		SetBreakValue(curBreakValue);
	}

	void CP_Boss::SetBreakValue(float _set)
	{
		// �Z�b�g����
		curBreakValue = _set;

		if (curBreakValue >= maxBreakValue)
		{
			curBreakValue = maxBreakValue;
			OnBreak();
		}
		else if (curBreakValue <= Mathf::epsilon)	// 0�ɂȂ�����
		{
			curBreakValue = 0.0f;
			OnBreakEnd();
		}

		// �X���C�_�[�ɔ��f
		pBreakBar->SetCurrentValue(curBreakValue);
	}

	void CP_Boss::OnBreak()
	{
		isBreaking = true;
		pAnimation->SetBool(BREAK_ANIMPARAM, true);
	}

	void CP_Boss::OnBreakEnd()
	{
		isBreaking = false;
		pAnimation->SetBool(BREAK_ANIMPARAM, false);
	}

	bool CP_Boss::OnDamageBehavior(const AttackInformation& _attackInfo,
		const DXSimp::Vector3& _attackerPos)
	{
		if (!CP_Enemy::OnDamageBehavior(_attackInfo, _attackerPos)) return false;

		// �u���C�N�l�����Z
		const PlayerAttackInformation* pPlayerAtkInfo =
			static_cast<const PlayerAttackInformation*>(&_attackInfo);
		AddBreakValue(pPlayerAtkInfo->GetBreakValue());

		pActionController->OnDamage(_attackInfo, _attackerPos);

		return true;
	}

	void CP_Boss::OnDeathBehavior()
	{
		CP_Enemy::OnDeathBehavior();

		// �v���C���[�������ɂ���
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->OnPlayerWin();
		}

		// �A�j���[�V�����ɒʒm
		if (pAnimation)
			pAnimation->SetTrigger(DEAD_ANIMPARAM);

	}

	float CP_Boss::GetBreakValue() const
	{
		return curBreakValue;
	}

	void CP_Boss::ImGuiDebug()
	{
		// �I�u�W�F�N�g�����Z�b�g
		static char input[IM_INPUT_BUF];
		ImGui::InputText("ObjName", input, IM_INPUT_BUF);

		ImGui::Text("Weapon:%s", weaponObjName.c_str());
		if (ImGui::Button("Weapon"))
			weaponObjName = input;

		ImGui::Text("HpBar:%s", hpBarObjName.c_str());
		if (ImGui::Button("HpBar"))
			hpBarObjName = input;

		ImGui::Text("BreakBar:%s", breakBarObjName.c_str());
		if (ImGui::Button("BreakBar"))
			breakBarObjName = input;

		ImGui::Begin("Boss");

		CP_Enemy::ImGuiDebug();
		ImGuiMethod::Text("IsBreaking", isBreaking);
		pActionController->ImGuiCall();

		ImGui::End();
	}
}