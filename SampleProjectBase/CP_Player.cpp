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
	CP_Character::Awake();

	//// モデル関係
	CP_MeshRenderer* pMeshRenderer = gameObject->GetComponent<CP_MeshRenderer>();
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
}

void CP_Player::Start()
{
	CP_Character::Start();

	//アニメーション関係生成
	CP_Animation* pAnimation = gameObject->GetComponent<CP_Animation>();

	// アニメーション変更オブザーバーを追加
	pAnimation->AddChangeAnimObserver(pActionController->GetChangeAnimObserver());

	// アクションコントローラー開始処理
	pActionController->Begin(*pAnimation);
}

void CP_Player::Update()
{
	CP_Character::Update();

	pActionController->Update();
}

void CP_Player::ImGuiSetting()
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

nlohmann::json CP_Player::Save()
{
	auto data = CP_Character::Save();
	data["actionController"] = pActionController->Save();

	return data;
}

void CP_Player::Load(const nlohmann::json& _data)
{
	CP_Character::Load(_data);

	nlohmann::json actionControllerData;
	if (HashiTaku::LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);
}

void CP_Player::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo)
{
}

void CP_Player::OnDeathBehavior()
{
}

void CP_Player::Copy(const CP_Player& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);
}
