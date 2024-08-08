#include "pch.h"
#include "CP_Player.h"

#include "ComponentDefine.h"
#include "GameObject.h"

void CP_Player::Init()
{
	name = "Player";

	pAnimController = std::make_unique<PlayerAnimController>();

	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh("Knight D Pelegrini");
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
	CP_Animation* pAnimation = gameObject->AddComponent<CP_Animation>();
	pAnimation->SetAnimationController(*pAnimController);

	pActionController = std::make_unique<PlayerActionController>(*gameObject);
}

void CP_Player::Update()
{
	pActionController->Update();
}

void CP_Player::ImGuiSetting()
{
	pActionController->ImGuiSetting();
}
