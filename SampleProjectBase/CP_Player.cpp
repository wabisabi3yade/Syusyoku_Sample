#include "pch.h"
#include "CP_Player.h"

#include "ComponentDefine.h"
#include "GameObject.h"

void CP_Player::Init()
{
	pAnimController = std::make_unique<PlayerAnimController>();

	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh("Knight D Pelegrini");
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
	CP_Animation* pAnimation = gameObject->AddComponent<CP_Animation>();
	pAnimation->SetAnimationController(*pAnimController);
}
