#include "pch.h"
#include "CP_Player.h"

#include "ComponentDefine.h"
#include "GameObject.h"

#include "SkeletalMesh.h"

using namespace DirectX::SimpleMath;

CP_Player::CP_Player()
	: pAnimation(nullptr)
{
}

CP_Player::CP_Player(const CP_Player& _other)
{
	Copy(_other);
}

CP_Player& CP_Player::operator=(const CP_Player& _other)
{
	Copy(_other);

	return *this;
}

void CP_Player::Init()
{
	// アクションコントローラー作成
	pActionController = std::make_unique<PlayerActionController>(*gameObject);
}

void CP_Player::Awake()
{
	//// モデル関係
	CP_MeshRenderer* pMeshRenderer = gameObject->GetComponent<CP_MeshRenderer>();
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
}

void CP_Player::Start()
{
	//アニメーション関係生成
	CP_Animation* pAnimation = gameObject->GetComponent<CP_Animation>();

	// アクションコントローラー開始処理
	pActionController->Begin(*pAnimation);
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

	pActionController->ImGuiCall();

	ImGui::End();
}

nlohmann::json CP_Player::Save()
{
	auto data = Component::Save();
	data["actionController"] = pActionController->Save();

	return data;
}

void CP_Player::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	nlohmann::json actionControllerData;
	if (HashiTaku::LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);
}

void CP_Player::Copy(const CP_Player& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
}
