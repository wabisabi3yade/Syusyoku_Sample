#include "pch.h"
#include "CP_Boss.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

constexpr auto CAN_ATTACK_ANIMPARAM("attackCollision");	// 攻撃可能フラグのアニメパラム名

CP_Boss::CP_Boss() :
	pAnimation(nullptr), pWeapon(nullptr), pCanAttack(nullptr)
{
}

void CP_Boss::Init()
{
	CP_Enemy::Init();

	pActionController = std::make_unique<BossActionController>(*this);
}

void CP_Boss::SetAttackInfo(const HashiTaku::AttackInformation& _attackInfo)
{
	if (!pWeapon) return;

	pWeapon->SetAttackInfo(_attackInfo);
}

nlohmann::json CP_Boss::Save()
{
	auto data = CP_Enemy::Save();

	data["actionController"] = pActionController->Save();
	data["weaponName"] = weaponObjName;

	return data;
}

void CP_Boss::Load(const nlohmann::json& _data)
{
	CP_Enemy::Load(_data);

	HashiTaku::LoadJsonString("weaponName", weaponObjName, _data);

	nlohmann::json actionControllerData;
	if (HashiTaku::LoadJsonData("actionController", actionControllerData, _data))
		pActionController->Load(actionControllerData);
}

void CP_Boss::Awake()
{
	CP_Enemy::Awake();
}

void CP_Boss::Start()
{
	CP_Enemy::Start();

	// 武器を取得
	SetupWeapon();

	// コンポーネント取得
	pAnimation = GetGameObject().GetComponent<CP_Animation>();
	CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

	// プレイヤーを取得する
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pActionController->SetPlayer(*pBattle->GetPlayerObject());
	}
	
	// アニメーションパラメータのアドレスを取得
	pCanAttack = pAnimation->GetParameterPointer<bool>(CAN_ATTACK_ANIMPARAM);

	// アクションコントローラー初期化s
	pActionController->Init(pAnimation, pRb);
}

void CP_Boss::Update()
{
	CP_Enemy::Update();

	pActionController->UpdateCall();

	// 攻撃判定を更新
	UpdateAttackCollision();
}

void CP_Boss::Draw()
{
	CP_Enemy::Draw();

#ifdef EDIT
	// アクション側で表示させたいデバッグ描画
	pActionController->DebugDisplay();
#endif // EDIT
}

void CP_Boss::UpdateAttackCollision()
{
#ifdef EDIT
	if (!pWeapon || !pCanAttack) return;
#endif // EDIT

	// 武器の攻撃判定をセットする
	pWeapon->SetIsAttackCollision(*pCanAttack);
}

void CP_Boss::SetupWeapon()
{
	auto pWeaponObj = InSceneSystemManager::GetInstance()->
		GetSceneObjects().GetSceneObject(weaponObjName);
	if (!pWeaponObj) return;

	// 武器コンポーネントを取得
	if (CP_Weapon* pGetWeapon = pWeaponObj->GetComponent<CP_Weapon>())
	{
		pWeapon = pGetWeapon;
		// 自分の座標を渡す
		pWeapon->SetHaveObjPosPointer(&GetTransform().GetPosition());
	}
}

bool CP_Boss::GetCanUpdate()
{
	return true;
}

void CP_Boss::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	CP_Enemy::OnDamageBehavior(_attackInfo, _attackerPos);

	pActionController->OnDamage(_attackInfo, _attackerPos);
}

void CP_Boss::ImGuiDebug()
{
	CP_Enemy::ImGuiDebug();

	// 武器オブジェクト名を入力
	static char input[IM_INPUT_BUF];
	ImGui::InputText("Weapon", input, IM_INPUT_BUF);
	if (ImGui::Button("Set"))
	{
		weaponObjName = input;
	}
	
	pActionController->ImGuiCall();
}
