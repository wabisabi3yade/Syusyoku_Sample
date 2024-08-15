#include "pch.h"
#include "CP_Player.h"

#include "ComponentDefine.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

void CP_Player::Init()
{
	name = "Player";

	// モデル関係
	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh("Knight D Pelegrini");
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
	pMeshRenderer->SetOffsetAngle(Vector3(0.f, 180.f, 0.f));

	// アニメーション関係生成
	CP_Animation* pAnimation = gameObject->AddComponent<CP_Animation>();
	pAnimController = std::make_unique<PlayerAnimController>();
	pAnimation->SetAnimationController(*pAnimController);

	// アクションコントローラー作成
	pActionController = std::make_unique<PlayerActionController>(*gameObject, *pAnimController);

	// アニメーションオブザーバー作成
	pAnimObserver = std::make_unique<PlayerAnimObserver>(*pActionController);

	// サブジェクトに渡す
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
