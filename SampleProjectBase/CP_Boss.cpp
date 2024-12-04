#include "pch.h"
#include "CP_Boss.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

constexpr auto CAN_ATTACK_ANIMPARAM("attackCollision");	// �U���\�t���O�̃A�j���p������
constexpr auto DEAD_ANIMPARAM("deadTrigger");	// ���񂾂Ƃ��̃A�j���p������

CP_Boss::CP_Boss() :
	pAnimation(nullptr), pWeapon(nullptr), pHpBar(nullptr), pCanAttack(nullptr)
{
}

void CP_Boss::Init()
{
	CP_Enemy::Init();

	pActionController = std::make_unique<BossActionController>(*this);
}

void CP_Boss::SetAttackInfo(const HashiTaku::AttackInformation& _attackInfo)
{
	if (!pWeapon) return;

	pWeapon->SetAttackInfo(_attackInfo);
}

nlohmann::json CP_Boss::Save()
{
	auto data = CP_Enemy::Save();

	data["actionController"] = pActionController->Save();
	data["weaponName"] = weaponObjName;
	data["hpBarName"] = hpBarObjName;

	return data;
}

void CP_Boss::Load(const nlohmann::json& _data)
{
	CP_Enemy::Load(_data);

	HashiTaku::LoadJsonString("weaponName", weaponObjName, _data);
	HashiTaku::LoadJsonString("hpBarName", hpBarObjName, _data);

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
		pHpBar->SetMaxValue(maxHP);
		pHpBar->SetCurrentValue(currentHP);
	}
}

void CP_Boss::SetCurrentHP(float _setHp)
{
	CP_Character::SetCurrentHP(_setHp);

	// �̗̓o�[�ɂ����f
	if (pHpBar)
		pHpBar->SetCurrentValue(currentHP);
}

void CP_Boss::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	CP_Enemy::OnDamageBehavior(_attackInfo, _attackerPos);

	pActionController->OnDamage(_attackInfo, _attackerPos);
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

void CP_Boss::ImGuiDebug()
{
	if (!ImGui::Begin("Boss")) return;

	CP_Enemy::ImGuiDebug();

	// �I�u�W�F�N�g�����Z�b�g
	static char input[IM_INPUT_BUF];
	ImGui::InputText("ObjName", input, IM_INPUT_BUF);

	ImGui::Text("Weapon:%s", weaponObjName.c_str());
	if (ImGui::Button("Weapon"))
		weaponObjName = input;

	ImGui::Text("HpBa:r%s", hpBarObjName.c_str());
	if (ImGui::Button("HpBar"))
		hpBarObjName = input;

	pActionController->ImGuiCall();

	ImGui::End();
}
