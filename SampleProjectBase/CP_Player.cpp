#include "pch.h"
#include "CP_Player.h"
#include "ComponentDefine.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "InSceneSystemManager.h"
#include "Geometory.h"

using namespace DirectX::SimpleMath;

// 攻撃判定のアニメーションパラメータ名
constexpr auto ATKCOL_ANIMPARAM("attackCollision");
// 死んだときのアニメーションパラメータ名
constexpr auto DEAD_ANIMPARAM("deadTrigger");

CP_Player::CP_Player() :
	pAnimation(nullptr),
	pWeapon(nullptr),
	pCameraMove(nullptr),
	pHpSlider(nullptr),
	pGuardSlider(nullptr),
	pAttackCollisionFlag(nullptr),
	hitStopBeforeAnimSpeed(0.0f),
	curGuardGage(0.0f),
	maxGuardGage(100.0f)
{
}

void CP_Player::SetAttackInfo(const HashiTaku::AttackInformation& _setAttackInfo)
{
	if (!pWeapon) return;

	pWeapon->SetAttackInfo(_setAttackInfo);

	pAnimation->SetBool(ATKCOL_ANIMPARAM, false);
}

void CP_Player::Init()
{
	CP_Character::Init();

	// アクションコントローラー作成
	pAction = std::make_unique<PlayerAction>(*this);
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

	// 必要オブジェクトをセット
	SetRequireObject();

	CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

	//アニメーション関係生成
	pAnimation = gameObject->GetComponent<CP_Animation>();
	pAttackCollisionFlag = pAnimation->GetParameterPointer<bool>(ATKCOL_ANIMPARAM);

	// アクションコントローラー開始処理
	pAction->Init(pAnimation, pRb);
}

void CP_Player::Update()
{
	if (!CanUpdate()) return;

	CP_Character::Update();

	// アクション周りを更新
	pAction->Update();

	// 武器の当たり判定を更新
	SetWeaponAttackFlag();
}

void CP_Player::Draw()
{
	CP_Character::Draw();
}

void CP_Player::OnDestroy()
{
	CP_Character::OnDestroy();

	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->RemovePlayer(*this);
	}
}

bool CP_Player::CanUpdate()
{
	if (GetDead()) return false;

	return true;
}

void CP_Player::SetRequireObject()
{
	SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

	// 武器コンポーネントを取得
	GameObject* pFindObj = sceneObjs.GetSceneObject(weaponObjName);
	if (pFindObj)
		pWeapon = pFindObj->GetComponent<CP_Weapon>();

	// カメラ移動クラス
	pFindObj = sceneObjs.GetSceneObject(cameraObjName);
	if (pFindObj)
		pCameraMove = pFindObj->GetComponent<CP_CameraMove>();

	// 体力バー
	pFindObj = sceneObjs.GetSceneObject(hpBarObjName);
	if (pFindObj)
	{
		pHpSlider = pFindObj->GetComponent<IUISlider>();
		pHpSlider->SetMaxValue(maxHP);
		pHpSlider->SetCurrentValue(currentHP);
	}

	// ガードバー
	pFindObj = sceneObjs.GetSceneObject(guardBarObjName);
	if (pFindObj)
	{
		pGuardSlider = pFindObj->GetComponent<IUISlider>();
		pGuardSlider->SetMaxValue(maxGuardGage);
		pGuardSlider->SetCurrentValue(curGuardGage);
	}

}

void CP_Player::OnHitStopBegin()
{
	CP_Character::OnHitStopBegin();
}

void CP_Player::OnHitStopEnd()
{
	CP_Character::OnHitStopEnd();
}

void CP_Player::AddGuardGage(float _addGage)
{
	curGuardGage += _addGage;	// 足す
	curGuardGage = std::clamp(curGuardGage, 0.0f, maxGuardGage);

	if (pGuardSlider)
		pGuardSlider->SetCurrentValue(curGuardGage);
}

void CP_Player::ResetGuardGage()
{
	curGuardGage = 0.0f;

	if (pGuardSlider)
		pGuardSlider->SetCurrentValue(0.0f);
}

void CP_Player::ImGuiDebug()
{
	CP_Character::ImGuiDebug();
	ImGuiFindObj();

	static bool isWindow = true;
	if (ImGui::Button("Window"))
		isWindow = !isWindow;
	if (!isWindow) return;

	ImGui::Begin("Action", &isWindow);
	pAction->ImGuiCall();
	ImGui::End();
}

void CP_Player::ImGuiFindObj()
{
#ifdef  EDIT

	// 武器オブジェクト名を入力
	static char input[IM_INPUT_BUF];
	ImGui::InputText("ObjName", input, IM_INPUT_BUF);

	std::string text = "Weapon:" + weaponObjName;
	ImGui::Text(text.c_str());
	if (ImGui::Button("Set Weapon"))
	{
		weaponObjName = input;
	}

	text = "Camera:" + cameraObjName;
	ImGui::Text(text.c_str());
	if (ImGui::Button("Set Camera"))
	{
		cameraObjName = input;
	}

	text = "HpBar:" + hpBarObjName;
	ImGui::Text(text.c_str());
	if (ImGui::Button("Set HpBar"))
	{
		hpBarObjName = input;
	}

	text = "GuardBar:" + guardBarObjName;
	ImGui::Text(text.c_str());
	if (ImGui::Button("Set GuardBar"))
	{
		guardBarObjName = input;
	}

#endif //  EDIT
}

bool CP_Player::GetIsAttackFlag() const
{
	return *pAttackCollisionFlag;
}

nlohmann::json CP_Player::Save()
{
	auto data = CP_Character::Save();
	data["action"] = pAction->Save();

	data["weaponObjName"] = weaponObjName;
	data["camObjName"] = cameraObjName;
	data["hpBarObjName"] = hpBarObjName;
	data["guardBarObjName"] = guardBarObjName;

	return data;
}

void CP_Player::Load(const nlohmann::json& _data)
{
	CP_Character::Load(_data);

	nlohmann::json actionData;
	if (HashiTaku::LoadJsonData("action", actionData, _data))
		pAction->Load(actionData);

	HashiTaku::LoadJsonString("weaponObjName", weaponObjName, _data);
	HashiTaku::LoadJsonString("camObjName", cameraObjName, _data);
	HashiTaku::LoadJsonString("hpBarObjName", hpBarObjName, _data);
	HashiTaku::LoadJsonString("guardBarObjName", guardBarObjName, _data);
}

void CP_Player::SetWeaponAttackFlag()
{
#ifdef EDIT
	if (!pWeapon || !pAttackCollisionFlag) return;
#endif // EDIT

	// 武器の攻撃判定をセットする
	pWeapon->SetIsAttackCollision(*pAttackCollisionFlag);
}

void CP_Player::DecadePlayerHp(float _damageVal)
{
	// 体力を減らす
	currentHP -= _damageVal;

	// スライダーにも反映
	if (pHpSlider)
		pHpSlider->SetCurrentValue(currentHP);
}

void CP_Player::OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos)
{
	bool isAcceptDamage = false;	// アクション内でダメージ受けているかチェック
	pAction->OnDamage(_attackInfo, _attackerPos, &isAcceptDamage);

	// ダメージ受けていたら
	if (isAcceptDamage)
	{
		// 体力を減らす
		DecadePlayerHp(_attackInfo.GetDamageValue());
	}
}

void CP_Player::OnDeathBehavior()
{
	// 敗北
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->OnPlayerLose();
	}

	if (pAnimation)
		pAnimation->SetTrigger(DEAD_ANIMPARAM);
}