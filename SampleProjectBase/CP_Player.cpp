#include "pch.h"
#include "CP_Player.h"
#include "ComponentDefine.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "InSceneSystemManager.h"
#include "Geometory.h"

namespace HashiTaku
{
	// �U������̃A�j���[�V�����p�����[�^��
	constexpr auto ATKCOL_ANIMPARAM("attackCollision");
	// ���񂾂Ƃ��̃A�j���[�V�����p�����[�^��
	constexpr auto DEAD_ANIMPARAM("deadTrigger");

	CP_Player::CP_Player() :
		pAnimation(nullptr),
		pWeapon(nullptr),
		pCameraMove(nullptr),
		pHpSlider(nullptr),
		pAttackCollisionFlag(nullptr),
		hitStopBeforeAnimSpeed(0.0f),
		isDebugInvicible(false)
	{
	}

	void CP_Player::SetAttackInfo(const AttackInformation& _setAttackInfo)
	{
		if (!pWeapon) return;

		pWeapon->SetAttackInfo(_setAttackInfo);

		pAnimation->SetBool(ATKCOL_ANIMPARAM, false);
	}

	void CP_Player::Init()
	{
		CP_Character::Init();

		// �A�N�V�����R���g���[���[�쐬
		pAction = std::make_unique<PlayerAction>(*this);
	}

	void CP_Player::Awake()
	{
		CP_Character::Awake();

		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->SetPlayer(*this);
		}
	}

	void CP_Player::Start()
	{
		CP_Character::Start();

		// �K�v�I�u�W�F�N�g���Z�b�g
		SetRequireObject();

		CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

		//�A�j���[�V�����֌W����
		pAnimation = gameObject->GetComponent<CP_Animation>();
		pAttackCollisionFlag = pAnimation->GetParameterPointer<bool>(ATKCOL_ANIMPARAM);

		// �A�N�V�����R���g���[���[�J�n����
		pAction->Init(pAnimation, pRb);
	}

	void CP_Player::Update()
	{
		if (!CanUpdate()) return;

		CP_Character::Update();

		// �A�N�V����������X�V
		pAction->Update();

		// ����̓����蔻����X�V
		SetWeaponAttackFlag();
	}

	void CP_Player::Draw()
	{
		CP_Character::Draw();
	}

	void CP_Player::OnDestroy()
	{
		CP_Character::OnDestroy();

		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->RemovePlayer(*this);
		}
	}

	bool CP_Player::CanUpdate()
	{
		if (GetDead()) return false;

		return true;
	}

	void CP_Player::SetRequireObject()
	{
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

		// ����R���|�[�l���g���擾
		GameObject* pFindObj = sceneObjs.GetSceneObject(weaponObjName);
		if (pFindObj)
			pWeapon = pFindObj->GetComponent<CP_Weapon>();

		// �J�����ړ��N���X
		pFindObj = sceneObjs.GetSceneObject(cameraObjName);
		if (pFindObj)
			pCameraMove = pFindObj->GetComponent<CP_CameraMove>();

		// �̗̓o�[
		pFindObj = sceneObjs.GetSceneObject(hpBarObjName);
		if (pFindObj)
		{
			pHpSlider = pFindObj->GetComponent<IUISlider>();
			pHpSlider->SetMaxValue(maxHP);
			pHpSlider->SetCurrentValue(currentHP);
		}
	}

	void CP_Player::OnHitStopBegin()
	{
		CP_Character::OnHitStopBegin();
	}

	void CP_Player::OnHitStopEnd()
	{
		CP_Character::OnHitStopEnd();
	}

	void CP_Player::ImGuiDebug()
	{
		CP_Character::ImGuiDebug();
		ImGuiFindObj();
		ImGui::Checkbox("DebugInvicible", &isDebugInvicible);

		static bool isWindow = true;
		if (ImGui::Button("Window"))
			isWindow = !isWindow;
		if (!isWindow) return;

		ImGui::Begin("Action", &isWindow);
		pAction->ImGuiCall();
		ImGui::End();
	}

	void CP_Player::ImGuiFindObj()
	{
#ifdef  EDIT

		// ����I�u�W�F�N�g�������
		static char input[IM_INPUT_BUF];
		ImGui::InputText("ObjName", input, IM_INPUT_BUF);

		std::string text = "Weapon:" + weaponObjName;
		ImGui::Text(text.c_str());
		if (ImGui::Button("Set Weapon"))
		{
			weaponObjName = input;
		}

		text = "Camera:" + cameraObjName;
		ImGui::Text(text.c_str());
		if (ImGui::Button("Set Camera"))
		{
			cameraObjName = input;
		}

		text = "HpBar:" + hpBarObjName;
		ImGui::Text(text.c_str());
		if (ImGui::Button("Set HpBar"))
		{
			hpBarObjName = input;
		}
#endif //  EDIT
	}

	bool CP_Player::GetIsAttackFlag() const
	{
		return *pAttackCollisionFlag;
	}

	json CP_Player::Save()
	{
		auto data = CP_Character::Save();
		data["action"] = pAction->Save();
		data["weaponObjName"] = weaponObjName;
		data["camObjName"] = cameraObjName;
		data["hpBarObjName"] = hpBarObjName;
#ifdef EDIT
		data["debugInvicible"] = isDebugInvicible;
#endif // EDIT
		return data;
	}

	void CP_Player::Load(const json& _data)
	{
		CP_Character::Load(_data);

		json actionData;
		if (LoadJsonData("action", actionData, _data))
			pAction->Load(actionData);
		LoadJsonString("weaponObjName", weaponObjName, _data);
		LoadJsonString("camObjName", cameraObjName, _data);
		LoadJsonString("hpBarObjName", hpBarObjName, _data);

#ifdef EDIT
		LoadJsonBoolean("debugInvicible", isDebugInvicible, _data);
#endif // EDIT

	}

	void CP_Player::SetWeaponAttackFlag()
	{
#ifdef EDIT
		if (!pWeapon || !pAttackCollisionFlag) return;
#endif // EDIT

		// ����̍U��������Z�b�g����
		pWeapon->SetIsAttackCollision(*pAttackCollisionFlag);
	}

	void CP_Player::DecadePlayerHp(float _damageVal)
	{
		// �̗͂����炷
		currentHP -= _damageVal;

		// �X���C�_�[�ɂ����f
		if (pHpSlider)
			pHpSlider->SetCurrentValue(currentHP);
	}

	bool CP_Player::OnDamageBehavior(const AttackInformation& _attackInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos)
	{
		// �f�o�b�O���G
		if (isDebugInvicible) return false;

		bool isAcceptDamage = false;	// �A�N�V�������Ń_���[�W�󂯂Ă��邩�`�F�b�N
		pAction->OnDamage(_attackInfo, _attackerPos, &isAcceptDamage);

		// �_���[�W�󂯂Ă�����
		if (!isAcceptDamage) return false;
		// �̗͂����炷
		DecadePlayerHp(_attackInfo.GetDamageValue());

		return true;
	}

	void CP_Player::OnDeathBehavior()
	{
		// �s�k
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->OnPlayerLose();
		}

		if (pAnimation)
			pAnimation->SetTrigger(DEAD_ANIMPARAM);
	}
}