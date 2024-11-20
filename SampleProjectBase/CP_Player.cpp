#include "pch.h"
#include "CP_Player.h"
#include "ComponentDefine.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "InSceneSystemManager.h"
#include "Geometory.h"

using namespace DirectX::SimpleMath;

constexpr auto ATKCOL_ANIMPARAM_NAME("attackCollision");

CP_Player::CP_Player()
	: pAnimation(nullptr), pWeapon(nullptr), hitStopBeforeAnimSpeed(0.0f), pAttackCollisionFlag(nullptr)
{
}

void CP_Player::SetWeapon(CP_Weapon& _setWeapon)
{
	pWeapon = &_setWeapon;
}

void CP_Player::SetAttackInfo(const HashiTaku::AttackInformation& _setAttackInfo)
{
	pWeapon->SetAttackInfo(_setAttackInfo);
	
	pAnimation->SetBool(ATKCOL_ANIMPARAM_NAME, false);
}

void CP_Player::Init()
{
	CP_Character::Init();

	// �A�N�V�����R���g���[���[�쐬
	pActionController = std::make_unique<PlayerActionController>(*this);
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

	//�A�j���[�V�����֌W����
	pAnimation = gameObject->GetComponent<CP_Animation>();
	pAttackCollisionFlag = pAnimation->GetParameterPointer<bool>(ATKCOL_ANIMPARAM_NAME);

	// �A�j���[�V�����ύX�I�u�U�[�o�[��ǉ�
	pAnimation->AddChangeAnimObserver(pActionController->GetChangeAnimObserver());

	CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

	// �A�N�V�����R���g���[���[�J�n����
	pActionController->Init(pAnimation, pRb);
}

void CP_Player::Update()
{
	//// �X�V�ł��邩�`�F�b�N
	//if (!GetCanUpdate()) return;

	CP_Character::Update();

	// �A�N�V����������X�V
	pActionController->UpdateCall();

	// ����̓����蔻����X�V
	SetWeaponAttackFlag();
}

void CP_Player::Draw()
{
	CP_Character::Draw();

//#ifdef  EDIT
//
//	Transform& t = GetTransform();
//	Geometory::AddLine(t.GetPosition(), t.GetPosition() + t.Forward() * 3.0f, Color(0, 0, 1));
//#endif //  EDIT
}

void CP_Player::OnDestroy()
{
	CP_Character::OnDestroy();

	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->RemovePlayer(*this);
	}
}

void CP_Player::OnHitStopBegin()
{
	CP_Character::OnHitStopBegin();

	//// �q�b�g�X�g�b�v�O�̍Đ����x���擾
	//hitStopBeforeAnimSpeed = pAnimation->GetControllerPlaySpeed();

	//// �q�b�g�X�g�b�v�����o���邽�߂ɃA�j���[�V�����̑��x��0�ɂ���
	//pAnimation->SetControllerPlaySpeed(0.0f);
}

void CP_Player::OnHitStopEnd()
{
	CP_Character::OnHitStopEnd();

	//// �A�j���[�V�����̑��x��߂�
	//pAnimation->SetControllerPlaySpeed(hitStopBeforeAnimSpeed);
}

void CP_Player::ImGuiDebug()
{
	ImGuiSetWeapon();

	static bool isWindow = true;

	if (ImGui::Button("Window"))
		isWindow = !isWindow;

	if (!isWindow) return;

	ImGui::Begin("Player", &isWindow);

	CP_Character::ImGuiDebug();
	pActionController->ImGuiCall();

	ImGui::End();
}

void CP_Player::ImGuiSetWeapon()
{
#ifdef  EDIT

	// �V�[��������I�u�W�F�N�g���擾����
	if (pWeapon)
	{
		ImGui::Text("Weapon is Setting");
	}

	static char input[IM_INPUT_BUF];
	ImGui::InputText("Weapon", input, IM_INPUT_BUF);

	if (ImGui::Button("Set"))
	{
		auto pWeaponObj = InSceneSystemManager::GetInstance()->GetSceneObjects().GetSceneObject(input);
		if (!pWeaponObj) return;

		// ����R���|�[�l���g���擾
		if (CP_Weapon* pGetWeapon = pWeaponObj->GetComponent<CP_Weapon>())
		{
			SetWeapon(*pGetWeapon);
		}
		else
		{
			HASHI_DEBUG_LOG("CP_Weapon������܂���");
		}
	}

	if (ImGui::Button("Release"))
		pWeapon = nullptr;

#endif //  EDIT
}

bool CP_Player::GetIsAttackFlag() const
{
	return *pAttackCollisionFlag;
}

nlohmann::json CP_Player::Save()
{
	auto data = CP_Character::Save();
	data["actionController"] = pActionController->Save();

	if (pWeapon)
		data["weaponObjName"] = pWeapon->GetGameObject().GetName();

	return data;
}

void CP_Player::Load(const nlohmann::json& _data)
{
	CP_Character::Load(_data);

	nlohmann::json actionControllerData;
	if (HashiTaku::LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);

	std::string weaponObjName;
	if (HashiTaku::LoadJsonString("weaponObjName", weaponObjName, _data))
	{
		auto pWeaponObj = InSceneSystemManager::GetInstance()->GetSceneObjects().GetSceneObject(weaponObjName);
		if (!pWeaponObj) return;

		// ����R���|�[�l���g���擾
		if (CP_Weapon* pGetWeapon = pWeaponObj->GetComponent<CP_Weapon>())
		{
			SetWeapon(*pGetWeapon);
		}
	}
}

void CP_Player::SetWeaponAttackFlag()
{
#ifdef EDIT
	if (!pWeapon || !pAttackCollisionFlag) return;
#endif // EDIT

	// ����̍U��������Z�b�g����
	pWeapon->SetIsAttackCollision(*pAttackCollisionFlag);
}

bool CP_Player::GetCanUpdate() const
{
	if (GetIsHitStopping()) return false;

	return true;
}

void CP_Player::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo)
{
	pActionController->OnDamage(_attackInfo);
}

void CP_Player::OnDeathBehavior()
{
}