#include "pch.h"
#include "CP_Boss.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

constexpr auto CAN_ATTACK_ANIMPARAM("attackCollision");	// 攻撃可能フラグのアニメパラム名
constexpr auto DEAD_ANIMPARAM("deadTrigger");	// 死んだときのアニメパラム名

CP_Boss::CP_Boss() :
	pAnimation(nullptr), pWeapon(nullptr), pHpBar(nullptr), pCanAttack(nullptr)
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
	data["hpBarName"] = hpBarObjName;

	return data;
}

void CP_Boss::Load(const nlohmann::json& _data)
{
	CP_Enemy::Load(_data);

	HashiTaku::LoadJsonString("weaponName", weaponObjName, _data);
	HashiTaku::LoadJsonString("hpBarName", hpBarObjName, _data);

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

	// オブジェクトを取得
	FindRequaireObject();

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
	if (!CanUpdate()) return;

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

bool CP_Boss::CanUpdate()
{
	if (GetDead()) return false;

	return true;
}

void CP_Boss::UpdateAttackCollision()
{
#ifdef EDIT
	if (!pWeapon || !pCanAttack) return;
#endif // EDIT

	// 武器の攻撃判定をセットする
	pWeapon->SetIsAttackCollision(*pCanAttack);
}

void CP_Boss::FindRequaireObject()
{
	SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

	// 武器コンポーネントを取得
	GameObject* pFindObj = sceneObjs.GetSceneObject(weaponObjName);
	if (!pFindObj) return;
	if (pWeapon = pFindObj->GetComponent<CP_Weapon>())
	{
		// 自分の座標を渡す
		pWeapon->SetHaveObjPosPointer(&GetTransform().GetPosition());
	}

	// 体力バー
	if (pFindObj = sceneObjs.GetSceneObject(hpBarObjName))
	{
		pHpBar = pFindObj->GetComponent<IUISlider>();
		pHpBar->SetMaxValue(maxHP);
		pHpBar->SetCurrentValue(currentHP);
	}
}

void CP_Boss::SetCurrentHP(float _setHp)
{
	CP_Character::SetCurrentHP(_setHp);

	// 体力バーにも反映
	if (pHpBar)
		pHpBar->SetCurrentValue(currentHP);
}

void CP_Boss::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	CP_Enemy::OnDamageBehavior(_attackInfo, _attackerPos);

	pActionController->OnDamage(_attackInfo, _attackerPos);
}

void CP_Boss::OnDeathBehavior()
{
	CP_Enemy::OnDeathBehavior();

	// プレイヤーを勝利にする
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->OnPlayerWin();
	}

	// アニメーションに通知
	if (pAnimation)
		pAnimation->SetTrigger(DEAD_ANIMPARAM);

}

void CP_Boss::ImGuiDebug()
{
	if (!ImGui::Begin("Boss")) return;

	CP_Enemy::ImGuiDebug();

	// オブジェクト名をセット
	static char input[IM_INPUT_BUF];
	ImGui::InputText("ObjName", input, IM_INPUT_BUF);

	ImGui::Text("Weapon:%s", weaponObjName.c_str());
	if (ImGui::Button("Weapon"))
		weaponObjName = input;

	ImGui::Text("HpBa:r%s", hpBarObjName.c_str());
	if (ImGui::Button("HpBar"))
		hpBarObjName = input;

	pActionController->ImGuiCall();

	ImGui::End();
}
