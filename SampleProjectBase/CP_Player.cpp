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
	//// モデル関係
	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh("Knight.fbx");
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");

	// アニメーション関係生成
	CP_Animation* pAnimation = gameObject->AddComponent<CP_Animation>();
	pAnimController = std::make_unique<PlayerAnimController>();
	pAnimation->SetAnimationController(*pAnimController);

	GameObject* pGo = &SceneFunction::ObjectFunc::CreateEmpty("Weapon");
	CP_Weapon* pWeapon = pGo->AddComponent<CP_Weapon>();
	SkeletalMesh* pSk = dynamic_cast<SkeletalMesh*>(pMeshRenderer->GetRenderMesh());
	pWeapon->SetGrabBone(pSk->GetBoneByName("mixamorig:LeftHand"));

	pGo->transform.SetParent(gameObject->transform);
}

void CP_Player::Start()
{
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
