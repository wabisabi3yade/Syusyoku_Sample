#include "pch.h"
#include "CP_Player.h"
#include "ComponentDefine.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "InSceneSystemManager.h"
#include "Geometory.h"

namespace HashiTaku
{
	// 攻撃判定のアニメーションパラメータ名
	constexpr auto ATKCOL_ANIMPARAM("attackCollision");
	// 死んだときのアニメーションパラメータ名
	constexpr auto DEAD_ANIMPARAM("deadTrigger");

	bool CP_Player::isDebugInvicible = false;

	CP_Player::CP_Player() :
		pAnimation(nullptr),
		pWeapon(nullptr),
		pCameraMove(nullptr),
		pHpSlider(nullptr),
		pAttackCollisionFlag(nullptr),
		pStylishUI(nullptr),
		hitStopBeforeAnimSpeed(0.0f),
		stylishPointRatioFromAtkDmg(10.0f),
		stylishPointRatioFromAcceptDmg(30.0f)
	{
	}

	void CP_Player::SetAttackInfo(AttackInformation& _setAttackInfo)
	{
		if (!pWeapon) return;

		pWeapon->SetAttackInfo(_setAttackInfo);

		pAnimation->SetBool(ATKCOL_ANIMPARAM, false);
	}

	void CP_Player::SetIsDebugInvincible(bool _setBool)
	{
		isDebugInvicible = _setBool;
	}

	void CP_Player::SetCurrentHP(float _setHp)
	{
		CP_Character::SetCurrentHP(_setHp);

		// スライダーにも反映
		if (pHpSlider)
			pHpSlider->SetCurrentValue(GetCurrentHP());
	}

	void CP_Player::Init()
	{
		CP_Character::Init();

		// アクションコントローラー作成
		pAction = std::make_unique<PlayerAction>(*this);
	}

	void CP_Player::OnAttacking(const AttackInformation& _atkInfo)
	{
		// ダメージ値に応じたスタイリッシュポイントを加算
		AddStylishPoint(_atkInfo.GetDamageValue() * stylishPointRatioFromAtkDmg);
	}

	void CP_Player::Awake()
	{
		CP_Character::Awake();
	}

	void CP_Player::Start()
	{
		CP_Character::Start();

		// バトルマネジャーに登録する
		if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
		{
			pBattle->SetPlayer(*this);
		}

		// 必要オブジェクトをセット
		SetRequireObject();

		CP_RigidBody* pRb = GetGameObject().GetComponent<CP_RigidBody>();

		//アニメーション関係生成
		pAnimation = gameObject->GetComponent<CP_Animation>();
		if (pAnimation)
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
		{
			if (pWeapon = pFindObj->GetComponent<CP_Weapon>())
			{
				// 自分の座標を渡す
				pWeapon->SetWeaponOwner(*this);
			}
		}

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

		pFindObj = sceneObjs.GetSceneObject(stylishUIName);
		if (pFindObj)
		{
			pStylishUI = pFindObj->GetComponent<CP_StylishUI>();
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

	void CP_Player::AddStylishPoint(float _addPoint)
	{
		if (!pStylishUI) return;

		if (_addPoint > 0.0f) // +なら
			pStylishUI->AddStylishPoint(_addPoint);
		else // -なら
			pStylishUI->DecadeStylishPoint(-_addPoint);
	}

	void CP_Player::ImGuiDebug()
	{
		CP_Character::ImGuiDebug();
		ImGuiFindObj();
		ImGui::Checkbox("DebugInvicible", &isDebugInvicible);
		ImGui::DragFloat("AtkStylishRatio", &stylishPointRatioFromAtkDmg, 0.01f, 0.0f, 10000.0f);
		ImGui::DragFloat("AcceptStylishRatio", &stylishPointRatioFromAcceptDmg, 0.01f, 0.0f, 10000.0f);

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

		text = "StylishUI:" + stylishUIName;
		ImGui::Text(text.c_str());
		if (ImGui::Button("Set StylishUI"))
		{
			stylishUIName = input;
		}
#endif //  EDIT
	}

	bool CP_Player::GetIsAttackFlag() const
	{
		return *pAttackCollisionFlag;
	}

	bool CP_Player::GetIsDebugInvincible()
	{
		return isDebugInvicible;
	}

	json CP_Player::Save()
	{
		auto data = CP_Character::Save();
		data["action"] = pAction->Save();
		data["weaponObjName"] = weaponObjName;
		data["camObjName"] = cameraObjName;
		data["hpBarObjName"] = hpBarObjName;
		data["stylishUIName"] = stylishUIName;
		data["stylishRatio"] = stylishPointRatioFromAtkDmg;
		data["stylishRatioAcceptDmg"] = stylishPointRatioFromAcceptDmg;
		return data;
	}

	void CP_Player::Load(const json& _data)
	{
		CP_Character::Load(_data);

		json actionData;
		if (LoadJsonData("action", actionData, _data))
			pAction->Load(actionData);
		LoadJsonString("weaponObjName", weaponObjName, _data);
		LoadJsonString("camObjName", cameraObjName, _data);
		LoadJsonString("hpBarObjName", hpBarObjName, _data);
		LoadJsonString("stylishUIName", stylishUIName, _data);
		LoadJsonFloat("stylishRatio", stylishPointRatioFromAtkDmg, _data);
		LoadJsonFloat("stylishRatioAcceptDmg", stylishPointRatioFromAcceptDmg, _data);
	}

	const DXSimp::Vector3& CP_Player::GetAttackerWorldPos() const
	{
		return GetTransform().GetPosition();
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

	bool CP_Player::OnDamageBehavior(AttackInformation& _attackInfo)
	{
		// アクション内でダメージ受けているかチェック
		bool isAcceptDamage = false;	// ダメージ受けたかフラグ
		pAction->OnDamage(_attackInfo, &isAcceptDamage);

		// ダメージ受けていたら
		if (!isAcceptDamage) return false;

		// デバッグ無敵なら
		if (isDebugInvicible) return false;

		return true;
	}

	void CP_Player::OnTakeDamage(const AttackInformation& _attackInfo, const DXSimp::Vector3& _contactPos)
	{
		CP_Character::OnTakeDamage(_attackInfo, _contactPos);

		// スタイリッシュポイントを減らす
		AddStylishPoint(-_attackInfo.GetDamageValue() * stylishPointRatioFromAcceptDmg);
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
}