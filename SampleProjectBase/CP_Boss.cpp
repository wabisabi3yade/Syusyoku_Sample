#include "pch.h"
#include "CP_Boss.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"
#include "PlayerAttackInformation.h"

namespace HashiTaku
{
	constexpr auto CAN_ATTACK_ANIMPARAM("attackCollision");	// 攻撃可能フラグのアニメパラム名
	constexpr auto DEAD_ANIMPARAM("deadTrigger");	// 死んだときのアニメパラム名
	constexpr auto BREAK_ANIMPARAM("isBreaking");	// ブレイク状態のアニメパラム名

	CP_Boss::CP_Boss() :
		pAnimation(nullptr),
		pWeapon(nullptr),
		pHpBar(nullptr),
		pBreakBar(nullptr),
		pCanAttack(nullptr),
		curBreakValue(0.0f),
		decadeBreakSpeed(10.0f),
		maxBreakValue(100.0f),
		isBreaking(false)
	{
	}

	void CP_Boss::Init()
	{
		CP_Enemy::Init();

		pActionController = std::make_unique<BossActionController>(*this);
	}

	void CP_Boss::SetAttackInfo(const AttackInformation& _attackInfo)
	{
		if (!pWeapon) return;

		pWeapon->SetAttackInfo(_attackInfo);
	}

	bool CP_Boss::GetIsBreaking() const
	{
		return isBreaking;
	}

	json CP_Boss::Save()
	{
		auto data = CP_Enemy::Save();

		data["actionController"] = pActionController->Save();
		data["weaponName"] = weaponObjName;
		data["hpBarName"] = hpBarObjName;
		data["breakBarName"] = breakBarObjName;

		return data;
	}

	void CP_Boss::Load(const json& _data)
	{
		CP_Enemy::Load(_data);

		LoadJsonString("weaponName", weaponObjName, _data);
		LoadJsonString("hpBarName", hpBarObjName, _data);
		LoadJsonString("breakBarName", breakBarObjName, _data);

		json actionControllerData;
		if (LoadJsonData("actionController", actionControllerData, _data))
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

		// ブレイク更新
		BreakUpdate();

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

	void CP_Boss::BreakUpdate()
	{
		if (!isBreaking) return;

		// 減らしていく
		curBreakValue -= decadeBreakSpeed * DeltaTime();

		// 0になったら
		if (curBreakValue < 0.0f)
		{
			curBreakValue = 0.0f;
			OnBreakEnd();
		}

		// スライダーに反映
		pBreakBar->SetCurrentValue(curBreakValue);
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
			if (pHpBar)
			{
				pHpBar->SetMaxValue(maxHP);
				pHpBar->SetCurrentValue(currentHP);
			}
		}

		// ブレイクバー
		if (pFindObj = sceneObjs.GetSceneObject(breakBarObjName))
		{
			pBreakBar = pFindObj->GetComponent<IUISlider>();
			if (pBreakBar)
			{
				pBreakBar->SetMaxValue(maxBreakValue);
				pBreakBar->SetCurrentValue(curBreakValue);
			}
		}
	}

	void CP_Boss::SetCurrentHP(float _setHp)
	{
		CP_Character::SetCurrentHP(_setHp);

		// 体力バーにも反映
		if (pHpBar)
			pHpBar->SetCurrentValue(currentHP);
	}

	void CP_Boss::AddBreakValue(float _add)
	{
		// ブレイク中は加算しない
		if (isBreaking) return;

		curBreakValue += _add;
		SetBreakValue(curBreakValue);
	}

	void CP_Boss::SetBreakValue(float _set)
	{
		// セットする
		curBreakValue = _set;

		if (curBreakValue >= maxBreakValue)
		{
			curBreakValue = maxBreakValue;
			OnBreak();
		}
		else if (curBreakValue <= Mathf::epsilon)	// 0になったら
		{
			curBreakValue = 0.0f;
			OnBreakEnd();
		}

		// スライダーに反映
		pBreakBar->SetCurrentValue(curBreakValue);
	}

	void CP_Boss::OnBreak()
	{
		isBreaking = true;
		pAnimation->SetBool(BREAK_ANIMPARAM, true);
	}

	void CP_Boss::OnBreakEnd()
	{
		isBreaking = false;
		pAnimation->SetBool(BREAK_ANIMPARAM, false);
	}

	bool CP_Boss::OnDamageBehavior(const AttackInformation& _attackInfo,
		const DXSimp::Vector3& _attackerPos)
	{
		if (!CP_Enemy::OnDamageBehavior(_attackInfo, _attackerPos)) return false;

		// ブレイク値を加算
		const PlayerAttackInformation* pPlayerAtkInfo =
			static_cast<const PlayerAttackInformation*>(&_attackInfo);
		AddBreakValue(pPlayerAtkInfo->GetBreakValue());

		pActionController->OnDamage(_attackInfo, _attackerPos);

		return true;
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

	float CP_Boss::GetBreakValue() const
	{
		return curBreakValue;
	}

	void CP_Boss::ImGuiDebug()
	{
		// オブジェクト名をセット
		static char input[IM_INPUT_BUF];
		ImGui::InputText("ObjName", input, IM_INPUT_BUF);

		ImGui::Text("Weapon:%s", weaponObjName.c_str());
		if (ImGui::Button("Weapon"))
			weaponObjName = input;

		ImGui::Text("HpBar:%s", hpBarObjName.c_str());
		if (ImGui::Button("HpBar"))
			hpBarObjName = input;

		ImGui::Text("BreakBar:%s", breakBarObjName.c_str());
		if (ImGui::Button("BreakBar"))
			breakBarObjName = input;

		ImGui::Begin("Boss");

		CP_Enemy::ImGuiDebug();
		ImGuiMethod::Text("IsBreaking", isBreaking);
		pActionController->ImGuiCall();

		ImGui::End();
	}
}