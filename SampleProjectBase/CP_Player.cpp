#include "pch.h"
#include "CP_Player.h"

#include "ComponentDefine.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void CP_Player::Init()
{
	name = "Player";

	// ���f���֌W
	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh("Knight D Pelegrini");
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
	pMeshRenderer->SetOffsetAngle(Vector3(0.f, 180.f, 0.f));

	// �A�j���[�V�����֌W����
	CP_Animation* pAnimation = gameObject->AddComponent<CP_Animation>();
	pAnimController = std::make_unique<PlayerAnimController>();
	pAnimation->SetAnimationController(*pAnimController);

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
