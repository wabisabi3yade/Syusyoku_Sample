#include "pch.h"
#include "CP_Player.h"

#include "ComponentDefine.h"
#include "GameObject.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

CP_Player::CP_Player(const CP_Player& _other)
{
	Copy(_other);
}

CP_Player& CP_Player::operator=(const CP_Player& _other)
{
	Copy(_other);

	return *this;
}

void CP_Player::Awake()
{
	////// ���f���֌W
	//CP_MeshRenderer* pMeshRenderer = gameObject->GetComponent<CP_MeshRenderer>();
	//pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	//pMeshRenderer->SetPixelShader("PS_Unlit");
}

void CP_Player::Start()
{
	// �A�j���[�V�����֌W����
	//CP_Animation* pAnimation = gameObject->GetComponent<CP_Animation>();
	//pAnimController = std::make_unique<PlayerAnimController>();
	//pAnimation->SetAnimationController(*pAnimController);

	// �A�N�V�����R���g���[���[�쐬
	pActionController = std::make_unique<PlayerActionController>(*gameObject, *pAnimController);

	// �A�j���[�V�����I�u�U�[�o�[�쐬
	pAnimObserver = std::make_unique<PlayerAnimObserver>(*pActionController);

	// �T�u�W�F�N�g�ɓn��
	pAnimController->AddObserver(*pAnimObserver);
}

void CP_Player::Update()
{
	pActionController->Update();
}

void CP_Player::ImGuiSetting()
{
	static bool isWindow = true;

	if (ImGui::Button("Window"))
		isWindow = !isWindow;

	if (!isWindow) return;

	ImGui::Begin("Player", &isWindow);

	pActionController->ImGuiSetting();

	ImGui::End();
}

nlohmann::json CP_Player::Save()
{
	auto data = Component::Save();

	return data;
}

void CP_Player::Load(const nlohmann::json& _data)
{
	Component::Load(_data);
}

void CP_Player::Copy(const CP_Player& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
}
