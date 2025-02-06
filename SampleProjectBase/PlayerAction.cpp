#include "pch.h"
#include "PlayerAction.h"
#include "PlayerGroundActionController.h"
#include "PlayerAirActionController.h"
#include "InSceneSystemManager.h"
#include "PlayerAnimReference.h"

namespace HashiTaku
{
	// アクションできる左スティックの入力量
	constexpr float CAN_ACTION_STICKINPUT(0.7f);

	PlayerAction::PlayerAction(CP_Player& _player):
		pPlayer(&_player),
		pGroundChecker(nullptr),
		pBattleManager(nullptr),
		pTargetAccepter(nullptr),
		pTargetCamera(nullptr),
		pCamera(nullptr),
		pAnimation(nullptr),
		pIsCanInput(nullptr),
		pIsCanAttack(nullptr),
		pIsCanCancel(nullptr),
		pIsCanMove(nullptr),
		isGround(true),
		prevIsGround(true),
		reserveAirTransition(GroundToAir::ToAirMove),
		reserveGroundTransition(AirToGround::AirToLanding),
		isTargeting(false),
		prevIsTargeting(false)
	{
		// 入力クラス取得
		InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
		pInput = &pInsceneSysytem->GetInput();

		// 各コントローラー生成
		stateNodeList[ActionPlace::Ground] = 
			std::make_unique<PlayerGroundActionController>(*this, _player);
		stateNodeList[ActionPlace::Air] = 
			std::make_unique<PlayerAirActionController>(*this, _player);

		// 地上から始める
		SetDefaultNode(ActionPlace::Ground);
	}

	PlayerAction::~PlayerAction()
	{
		// ターゲットする側に削除するように通知
		if (pTargetAccepter)
			pTargetAccepter->RemoveTargeter(*this);
	}

	void PlayerAction::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
	{
		InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
		pBattleManager = CP_BattleManager::GetInstance();
		pAnimation = _pAnimation;
		pCamera = &pInsceneSysytem->GetMainCamera();
		if (pCamera)
		{
			pTargetCamera = pCamera->GetGameObject().GetComponent<CP_CameraMove>();
		}
		if (pAnimation)
		{
			// アニメーションパラメータのアドレスを取得
			pIsCanCancel = pAnimation->GetParameterPointer<bool>(CANCEL_PARAMNAME);
			// 先行入力のアドレスを取得
			pIsCanInput = pAnimation->GetParameterPointer<bool>(INPUT_PARAMNAME);
			// 攻撃可能アドレスを取得
			pIsCanAttack = pAnimation->GetParameterPointer<bool>(CANATK_PARAMNAME);
			// 移動可能アドレスを取得
			pIsCanMove = pAnimation->GetParameterPointer<bool>(CANMOVE_PARAMNAME);
		}

		// 接地チェッカーを取得
		FindGroundChecker();

		// アクション初期化
		for (auto& node : stateNodeList)
		{
			PlayerActionController_Base& actCon = *GetActionController(node.first);

			actCon.Init(pAnimation, _pRigidBody);
		}

		// ステートマシンの初期化
		StateMachine_Base::Init();
		pCurActionController = GetActionController(currentStateKey);
	}

	void PlayerAction::Update()
	{
		// 現在のアクションコントローラーを更新
		StateMachine_Base::Update();

		// ターゲットの更新
		UpdateTargeting();

		// 地上判定の更新
		UpdateGround();
	}

	bool PlayerAction::ChangeNode(const ActionPlace& _changeKey, bool _isForceChange)
	{
		// アクションコントローラーを変更
		bool isSuccess = StateMachine_Base::ChangeNode(_changeKey, _isForceChange);
		if (!isSuccess) return false;	// 変更に成功しなかったなら終える

		// 更新
		pCurActionController = GetActionController(currentStateKey);

		// どの場所に移動したか？
		switch (currentStateKey)
		{
		case ActionPlace::Ground:
			OnAirToGround();
			break;
		case ActionPlace::Air:
			OnGroundToAir();
			break;

		default:
			break;
		}

		return true;
	}

	CP_Camera& PlayerAction::GetCamera()
	{
		return *pCamera;
	}

	const ITargetAccepter* PlayerAction::GetTargetAccepter() const
	{
		return pTargetAccepter;
	}

	const DXSimp::Vector2& PlayerAction::GetInputLeftStick() const
	{
		return  pInput->GetValue(GameInput::ValueType::Player_Move);
	}

	bool PlayerAction::IsInputVector(InputVector _checkVector)
	{
		// ボタン入力
		if (!pInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

		// ローリングできる左スティックの入力ができていない
		if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ACTION_STICKINPUT) return false;

		return true;
	}

	bool PlayerAction::GetIsTargeting() const
	{
		return isTargeting;
	}

	bool PlayerAction::GetCanInput() const
	{
#ifdef EDIT
		if (!pAnimation) return false;
		if (!pIsCanInput) return false;
#endif // EDIT

		return *pIsCanInput;
	}

	bool PlayerAction::GetCanCancel() const
	{
#ifdef EDIT
		if (!pAnimation) return false;
		if (!pIsCanCancel) return false;
#endif // EDIT

		return *pIsCanCancel;
	}

	bool PlayerAction::GetCanAttack() const
	{
#ifdef EDIT
		if (!pAnimation) return false;
		if (!pIsCanAttack) return false;
#endif // EDIT

		return *pIsCanAttack;
	}

	bool PlayerAction::GetCanMove() const
	{
#ifdef EDIT
		if (!pAnimation) return false;
		if (!pIsCanMove) return false;
#endif // EDIT

		return *pIsCanMove; 
	}

	bool PlayerAction::GetIsGround() const
	{
		return isGround;
	}

	void PlayerAction::SetTargetObject(ITargetAccepter& _targetObject)
	{
		pTargetAccepter = &_targetObject;
		pTargetAccepter->SetTargeter(*this);
	}

	void PlayerAction::RemoveNotify(const ITargetAccepter& _deathTargetObj)
	{
		// 死んだターゲット先が現在見ているオブジェクトだったら参照を消す
		if (!pTargetAccepter) return;
		if (pTargetAccepter != &_deathTargetObj) return;

		pTargetAccepter = nullptr;
	}

	void PlayerAction::OnDamage(AttackInformation& _atkInfo,
		bool* _pAcceptDamage)
	{
		pCurActionController->OnDamage(_atkInfo,
			_pAcceptDamage);
	}

	void PlayerAction::OnGroundEnter()
	{
		// 接地フラグとアニメーションの方をtrueする
		isGround = true;
		if (pAnimation)
		{
			pAnimation->SetBool(GROUND_PARAMNAME, isGround);
		}
		HASHI_DEBUG_LOG("着地");
	}

	void PlayerAction::OnGroundExit()
	{
		// 接地フラグとアニメーションの方をfalseする
		isGround = false;
		if (pAnimation)
		{
			pAnimation->SetBool(GROUND_PARAMNAME, isGround);
		}
		HASHI_DEBUG_LOG("離陸");
	}

	json PlayerAction::Save()
	{
		json data;

		data["groundCheckName"] = groundCheckerName;

		data["ground"] = GetActionController(ActionPlace::Ground)->Save();
		data["air"] = GetActionController(ActionPlace::Air)->Save();

		return data;
	}

	void PlayerAction::Load(const json& _data)
	{
		LoadJsonString("groundCheckName", groundCheckerName, _data);

		json loadData;
		if (LoadJsonData("ground", loadData, _data))
		{
			GetActionController(ActionPlace::Ground)->Load(loadData);
		}
		if (LoadJsonData("air", loadData, _data))
		{
			GetActionController(ActionPlace::Air)->Load(loadData);
		}
	}

	void PlayerAction::FindGroundChecker()
	{
		if (groundCheckerName.empty()) return;

		// 名前からオブジェクトを取得
		auto& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* go = sceneObjs.GetSceneObject(groundCheckerName);
		if (!go) return;

		// 接地チェッカーコンポーネントを取得
		pGroundChecker = go->GetComponent<CP_GroundCheck>();
		if (!pGroundChecker)
		{
			HASHI_DEBUG_LOG("CP_GroundCheckerがオブジェクトにありません");
			return;
		}

		// 接地チェッカーにセットする
		pGroundChecker->SetGroundNotifyer(*this);
	}

	bool PlayerAction::GetCanUpdate()
	{
		if (!pCamera) return false;

		return true;
	}

	void PlayerAction::OnBeginTargeting()
	{
		if (!pBattleManager) return;

		// 敵リストを取得する
		auto& enemyList = pBattleManager->GetEnemyList();
		u_int enemyCnt = static_cast<u_int>(enemyList.size());

		if (enemyCnt == 0) return;

		// ↓敵が複数体でるようにするならどのターゲットにするかの処理をここに書く
		// 今回はボス1体なのでそいつを見る
		ITargetAccepter* pTargetEnemy = *enemyList.begin();

		SetTargetObject(*pTargetEnemy);

		if (pTargetCamera)	// カメラにも伝える
			pTargetCamera->SetTargetObject(*pTargetEnemy);
	}

	void PlayerAction::OnEndTargeting()
	{
		if (!pTargetAccepter) return;

		if (pTargetCamera)	// カメラにも伝える
		{
			pTargetCamera->RemoveNotify(*pTargetAccepter);
			pTargetAccepter->RemoveTargeter(*pTargetCamera);
		}

		// ターゲットで参照するのをやめる
		pTargetAccepter->RemoveTargeter(*this);
		pTargetAccepter = nullptr;
	}

	void PlayerAction::UpdateTargeting()
	{
		// ボタン押されたらターゲット中
		isTargeting = pInput->GetButton(GameInput::ButtonType::Player_RockOn);

		// アニメーションパラメータにも送る
		pAnimation->SetBool(TARGET_PARAMNAME, isTargeting);

		if (isTargeting && !prevIsTargeting)	// ターゲットした瞬間
			OnBeginTargeting();
		else if (!isTargeting && prevIsTargeting)	// ターゲット解除の瞬間
			OnEndTargeting();

		// 次フレームの為に更新
		prevIsTargeting = isTargeting;
	}

	void PlayerAction::UpdateGround()
	{
		// 前フレームと同じなら処理しない
		if (isGround == prevIsGround) return;

		if (isGround) 	// 地上へ移行
			OnAirToGround();
		else	// 空中へ移行
			OnGroundToAir();

		prevIsGround = isGround;	// 更新
	}

	void PlayerAction::OnGroundToAir()
	{
		// 空中アクションコントローラーを取得
		PlayerActionController_Base* pAirController = GetActionController(ActionPlace::Air);

		// 空中のエントリー状態をを設定
		switch (reserveAirTransition)
		{
		case GroundToAir::ToAirMove:
		{
			int moveStateId = static_cast<int>(PlayerAirActionController::AirState::Move);
			pAirController->ChangeState(moveStateId, true);
		}
			
			break;

		default:
			break;
		}

		// デフォルトに戻す
		reserveAirTransition = GroundToAir::ToAirMove;
	}

	void PlayerAction::OnAirToGround()
	{
		// 空中アクションコントローラーを取得
		PlayerActionController_Base* pGroundController = 
			GetActionController(ActionPlace::Ground);

		// 地上のエントリー状態をを設定
		switch (reserveGroundTransition)
		{
		case AirToGround::AirToLanding:
		{
			int idleStateId = static_cast<int>(PlayerGroundActionController::GroundState::Idle);
			pGroundController->ChangeState(idleStateId, true);
		}
			
			break;

		case AirToGround::AirToGroundKnock:
		{
			int moveStateId = static_cast<int>(PlayerGroundActionController::GroundState::Move);
			pGroundController->ChangeState(moveStateId, true);
		}
			
			break;

		default:
			break;
		}

		// デフォルトに戻す
		reserveGroundTransition = AirToGround::AirToLanding;
	}

	PlayerActionController_Base* PlayerAction::GetActionController(ActionPlace _place)
	{
		PlayerActionController_Base* pActCon = nullptr;

#ifdef EDIT
		pActCon = dynamic_cast<PlayerActionController_Base*>(stateNodeList.at(_place).get());
		assert(pActCon && "アクションコントローラーが正常に取得できません");
		if (!pActCon) return nullptr;
#else
		pActCon = static_cast<PlayerActionController_Base*>(stateNodeList.at(_place).get());
#endif // EDIT

		return pActCon;
	}

	void PlayerAction::ImGuiDebug()
	{
		ImGuiMethod::Text("isGround", isGround);
		ImGuiMethod::Text("IsTargeting", isTargeting);
		ImGui::Text("Place:%s", std::string(magic_enum::enum_name(currentStateKey)).c_str());

		// 各アクションコントローラーの編集
		for (auto& node : stateNodeList)
		{
			PlayerActionController_Base* pActionCon = GetActionController(node.first);
			std::string placeStr = std::string(magic_enum::enum_name(node.first));

			if (!ImGuiMethod::TreeNode(placeStr)) continue;
			pActionCon->ImGuiCall();
			ImGui::TreePop();
		}
	}

	void PlayerAction::ImGuiGroundChecker()
	{
#ifdef EDIT
		char input[IM_INPUT_BUF];
		std::strcpy(input, groundCheckerName.c_str());
		if (ImGui::InputText("GroundChecker", input, IM_INPUT_BUF))
		{
			groundCheckerName = input;
		}
#endif // EDIT
	}
}