#include "pch.h"
#include "CP_Player.h"
#include "ComponentDefine.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

constexpr auto ATKCOL_ANIMPARAM_NAME("attackCollision");

CP_Player::CP_Player()
	: pAnimation(nullptr), pWeapon(nullptr), hitStopBeforeAnimSpeed(0.0f), pAttackCollisionFlag(nullptr)
{
}

void CP_Player::SetWeapon(CP_Weapon& _setWeapon)
{
	pWeapon = &_setWeapon;
}

void CP_Player::SetAttackInfo(const HashiTaku::AttackInformation& _setAttackInfo)
{
	pWeapon->SetAttackInfo(_setAttackInfo);
}

void CP_Player::Init()
{
	CP_Character::Init();

	// アクションコントローラー作成
	pActionController = std::make_unique<PlayerActionController>(*this);
}

void CP_Player::Awake()
{
	CP_Character::Awake();

	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->SetPlayer(*this);
	}
}

void CP_Player::Start()
{
	CP_Character::Start();

	//アニメーション関係生成
	pAnimation = gameObject->GetComponent<CP_Animation>();
	pAttackCollisionFlag = pAnimation->GetParameterPointer<bool>(ATKCOL_ANIMPARAM_NAME);

	// アニメーション変更オブザーバーを追加
	pAnimation->AddChangeAnimObserver(pActionController->GetChangeAnimObserver());

	// アクションコントローラー開始処理
	pActionController->Begin(*pAnimation);
}

void CP_Player::Update()
{
	// 更新できるかチェック
	if (!GetCanUpdate()) return;

	CP_Character::Update();

	// アクション周りを更新
	pActionController->Update();

	// 武器の当たり判定を更新
	SetWeaponAttackFlag();
}

void CP_Player::OnDestroy()
{
	CP_Character::OnDestroy();

	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->RemovePlayer(*this);
	}
}

void CP_Player::OnHitStopBegin()
{
	CP_Character::OnHitStopBegin();

	// ヒットストップ前の再生速度を取得
	hitStopBeforeAnimSpeed = pAnimation->GetControllerPlaySpeed();

	// ヒットストップを演出するためにアニメーションの速度を0にする
	pAnimation->SetControllerPlaySpeed(0.0f);
}

void CP_Player::OnHitStopEnd()
{
	CP_Character::OnHitStopEnd();

	// アニメーションの速度を戻す
	pAnimation->SetControllerPlaySpeed(hitStopBeforeAnimSpeed);
}

void CP_Player::ImGuiSetting()
{
	ImGuiSetWeapon();

	static bool isWindow = true;

	if (ImGui::Button("Window"))
		isWindow = !isWindow;

	if (!isWindow) return;

	ImGui::Begin("Player", &isWindow);

	CP_Character::ImGuiSetting();
	pActionController->ImGuiCall();

	ImGui::End();
}

void CP_Player::ImGuiSetWeapon()
{
#ifdef  EDIT

	// シーン内からオブジェクトを取得する
	if (pWeapon)
	{
		ImGui::Text("Weapon is Setting");
	}

	static char input[IM_INPUT_BUF];
	ImGui::InputText("Weapon", input, IM_INPUT_BUF);

	if (ImGui::Button("Set"))
	{
		auto pWeaponObj = InSceneSystemManager::GetInstance()->GetSceneObjects().GetSceneObject(input);
		if (!pWeaponObj) return;

		// 武器コンポーネントを取得
		if (CP_Weapon* pGetWeapon = pWeaponObj->GetComponent<CP_Weapon>())
		{
			SetWeapon(*pGetWeapon);
		}
		else
		{
			HASHI_DEBUG_LOG("CP_Weaponがありません");
		}
	}

	if (ImGui::Button("Release"))
		pWeapon = nullptr;

#endif //  EDIT
}

nlohmann::json CP_Player::Save()
{
	auto data = CP_Character::Save();
	data["actionController"] = pActionController->Save();

	if (pWeapon)
		data["weaponObjName"] = pWeapon->GetGameObject().GetName();

	return data;
}

void CP_Player::Load(const nlohmann::json& _data)
{
	CP_Character::Load(_data);

	nlohmann::json actionControllerData;
	if (HashiTaku::LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);

	std::string weaponObjName;
	if (HashiTaku::LoadJsonString("weaponObjName", weaponObjName, _data))
	{
		auto pWeaponObj = InSceneSystemManager::GetInstance()->GetSceneObjects().GetSceneObject(weaponObjName);
		if (!pWeaponObj) return;

		// 武器コンポーネントを取得
		if (CP_Weapon* pGetWeapon = pWeaponObj->GetComponent<CP_Weapon>())
		{
			SetWeapon(*pGetWeapon);
		}
	}
}

void CP_Player::SetWeaponAttackFlag()
{
#ifdef EDIT
	if (!pWeapon || !pAttackCollisionFlag) return;
#endif // EDIT

	// 武器の攻撃判定をセットする
	pWeapon->SetIsAttackCollision(*pAttackCollisionFlag);
}

bool CP_Player::GetCanUpdate() const
{
	if (GetIsHitStopping()) return false;

	return true;
}

void CP_Player::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo)
{
}

void CP_Player::OnDeathBehavior()
{
}