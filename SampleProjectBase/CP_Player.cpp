#include "pch.h"
#include "CP_Player.h"

#include "ComponentDefine.h"
#include "GameObject.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

HashiTaku::CP_Player::CP_Player()
	: pAnimation(nullptr)
{
}

HashiTaku::CP_Player::CP_Player(const CP_Player& _other)
{
	Copy(_other);
}

HashiTaku::CP_Player& HashiTaku::CP_Player::operator=(const CP_Player& _other)
{
	Copy(_other);

	return *this;
}

void HashiTaku::CP_Player::Init()
{
	// �A�N�V�����R���g���[���[�쐬
	pActionController = std::make_unique<PlayerActionController>(*gameObject);
}

void HashiTaku::CP_Player::Awake()
{
	//// ���f���֌W
	CP_MeshRenderer* pMeshRenderer = gameObject->GetComponent<CP_MeshRenderer>();
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
}

void HashiTaku::CP_Player::Start()
{
	//�A�j���[�V�����֌W����
	CP_Animation* pAnimation = gameObject->GetComponent<CP_Animation>();

	// �A�j���[�V�����ύX�I�u�U�[�o�[��ǉ�
	pAnimation->AddChangeAnimObserver(pActionController->GetChangeAnimObserver());

	// �A�N�V�����R���g���[���[�J�n����
	pActionController->Begin(*pAnimation);
}

void HashiTaku::CP_Player::Update()
{
	pActionController->Update();
}

void HashiTaku::CP_Player::ImGuiSetting()
{
	static bool isWindow = true;

	if (ImGui::Button("Window"))
		isWindow = !isWindow;

	if (!isWindow) return;

	ImGui::Begin("Player", &isWindow);

	CP_Character::ImGuiSetting();
	pActionController->ImGuiCall();

	ImGui::End();
}

nlohmann::json HashiTaku::CP_Player::Save()
{
	auto data = CP_Character::Save();
	data["actionController"] = pActionController->Save();

	return data;
}

void HashiTaku::CP_Player::Load(const nlohmann::json& _data)
{
	CP_Character::Load(_data);

	nlohmann::json actionControllerData;
	if (LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);
}

void HashiTaku::CP_Player::OnDamageBehavior(const AttackInformation& _attackInfo)
{
}

void HashiTaku::CP_Player::OnDeathBehavior()
{
}

void HashiTaku::CP_Player::Copy(const CP_Player& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
}
