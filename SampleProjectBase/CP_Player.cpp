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

void CP_Player::SetAttackInfo(const HashiTaku::AttackInformation& _setAttackInfo)
{
	if (!pWeapon) return;

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

	CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

	//�A�j���[�V�����֌W����
	pAnimation = gameObject->GetComponent<CP_Animation>();
	pAttackCollisionFlag = pAnimation->GetParameterPointer<bool>(ATKCOL_ANIMPARAM_NAME);

	// �A�j���[�V�����ύX�I�u�U�[�o�[��ǉ�
	pAnimation->AddChangeAnimObserver(pActionController->GetChangeAnimObserver());

	// �A�N�V�����R���g���[���[�J�n����
	pActionController->Init(pAnimation, pRb);

	// ����I�u�W�F�N�g���Z�b�g
	SetWeaponObject();
}

void CP_Player::Update()
{
	CP_Character::Update();

	// �A�N�V����������X�V
	pActionController->UpdateCall();

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

void CP_Player::SetWeaponObject()
{
	auto pWeaponObj = InSceneSystemManager::GetInstance()->
		GetSceneObjects().GetSceneObject(weaponObjName);
	if (!pWeaponObj) return;

	// ����R���|�[�l���g���擾
	if (CP_Weapon* pGetWeapon = pWeaponObj->GetComponent<CP_Weapon>())
	{
		pWeapon = pGetWeapon;
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

	// ����I�u�W�F�N�g�������
	static char input[IM_INPUT_BUF];
	ImGui::InputText("Weapon", input, IM_INPUT_BUF);
	if (ImGui::Button("Set"))
	{
		weaponObjName = input;
	}

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

	data["weaponObjName"] = weaponObjName;

	return data;
}

void CP_Player::Load(const nlohmann::json& _data)
{
	CP_Character::Load(_data);

	nlohmann::json actionControllerData;
	if (HashiTaku::LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);

	HashiTaku::LoadJsonString("weaponObjName", weaponObjName, _data);
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