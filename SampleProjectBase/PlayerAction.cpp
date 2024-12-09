#include "pch.h"
#include "PlayerAction.h"
#include "InSceneSystemManager.h"

// アクションできる左スティックの入力量
constexpr float CAN_ACTION_STICKINPUT(0.7f);

// 移動速度
//constexpr auto SPEEDRATIO_PARAMNAME{ "speed" };
// ターゲット中を表すアニメーションパラメータ
constexpr auto TARGET_PARAMNAME{ "targeting" };
// 接地しているかを表すアニメーションパラメータ
constexpr auto GROUND_PARAMNAME{ "isGround" };
// キャンセルできるかを表すアニメーションパラメータ
constexpr auto CANCEL_PARAMNAME{ "canCancel" };
// 先行入力できるかを表すアニメーションパラメータ
constexpr auto INPUT_PARAMNAME{ "canInput" };
// 攻撃できるかを表すアニメーションパラメータ
constexpr auto CANATK_PARAMNAME{ "canAttack" };
// 移動できるかを表すアニメーションパラメータ
constexpr auto CANMOVE_PARAMNAME{ "canMove" };

PlayerAction::PlayerAction(CP_Player& _player) :
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
	InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
	pPlayer = &_player;
	pInput = &pInsceneSysytem->GetInput();
	pGroundController = std::make_unique<PlayerGroundActionController>(*this, _player);
	pAirController = std::make_unique<PlayerAirActionController>(*this, _player);
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
	pGroundController->Init(pAnimation, _pRigidBody);
	pAirController->Init(pAnimation, _pRigidBody);

	pCurrentController = pGroundController.get();
	currentPlace = ActionPlace::Ground;
}

void PlayerAction::Update()
{
	pCurrentController->UpdateCall();

	UpdateTargeting();

	UpdateGround();
}

void PlayerAction::OnChangePlace()
{
}

CP_Camera& PlayerAction::GetCamera()
{
	return *pCamera;
}

const ITargetAccepter* PlayerAction::GetTargetAccepter() const
{
	return pTargetAccepter;
}

const DirectX::SimpleMath::Vector2& PlayerAction::GetInputLeftStick() const
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
	assert(pIsCanInput);
	return *pIsCanInput;
}

bool PlayerAction::GetCanCancel() const
{
	assert(pIsCanCancel);
	return *pIsCanCancel;
}

bool PlayerAction::GetCanAttack() const
{
	assert(pIsCanAttack);
	return *pIsCanAttack;
}

bool PlayerAction::GetCanMove() const
{
	assert(pIsCanMove);
	return *pIsCanMove; return false;
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

void PlayerAction::OnDamage(const HashiTaku::AttackInformation& _atkInfo,
	const DirectX::SimpleMath::Vector3& _attackerPos,
	bool* _pAcceptDamage)
{
	pCurrentController->OnDamage(_atkInfo,
		_attackerPos,
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

nlohmann::json PlayerAction::Save()
{
	nlohmann::json data;

	data["groundCheckName"] = groundCheckerName;

	data["ground"] = pGroundController->Save();
	data["air"] = pAirController->Save();

	return data;
}

void PlayerAction::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonString("groundCheckName", groundCheckerName, _data);

	nlohmann::json loadData;
	if (HashiTaku::LoadJsonData("ground", loadData, _data))
	{
		pGroundController->Load(loadData);
	}
	if (HashiTaku::LoadJsonData("air", loadData, _data))
	{
		pAirController->Load(loadData);
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

void PlayerAction::ActionUpdate()
{
	pCurrentController->UpdateCall();
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

	// 変更前の終了処理
	pCurrentController->OnEnd();

	if (isGround) 	// 地上へ移行
		OnAirToGround();
	else	// 空中へ移行
		OnGroundToAir();

	prevIsGround = isGround;	// 更新
}

void PlayerAction::OnGroundToAir()
{
	currentPlace = ActionPlace::Air;

	// 現在のコントローラーを変更
	pCurrentController = pAirController.get();

	// 空中のエントリー状態をを設定
	switch (reserveAirTransition)
	{
	case PlayerAction::GroundToAir::ToAirMove:
		pAirController->ChangeAirState(PlayerAirActionController::AirState::Move);
		break;

	default:
		break;
	}

	// デフォルトに戻す
	reserveAirTransition = GroundToAir::ToAirMove;
}

void PlayerAction::OnAirToGround()
{
	currentPlace = ActionPlace::Ground;

	// 現在のコントローラーを変更
	pCurrentController = pGroundController.get();

	// 地上のエントリー状態をを設定
	switch (reserveGroundTransition)
	{
	case AirToGround::AirToLanding:
		pGroundController->
			ChangeGroundState(PlayerGroundActionController::GroundState::Idle, true);
		break;
	case AirToGround::AirToGroundKnock:
		pGroundController->
			ChangeGroundState(PlayerGroundActionController::GroundState::Move, true);
		break;
	default:
		break;
	}

	// デフォルトに戻す
	reserveGroundTransition = AirToGround::AirToLanding;
}

void PlayerAction::ImGuiDebug()
{
	ImGuiMethod::Text("isGround", isGround);
	ImGuiMethod::Text("IsTargeting", isTargeting);
	ImGui::Text("Place:%s", std::string(magic_enum::enum_name(currentPlace)).c_str());

	if (ImGuiMethod::TreeNode("Ground"))
	{
		pGroundController->ImGuiCall();
		ImGui::TreePop();
	}

	if (ImGuiMethod::TreeNode("Air"))
	{
		pAirController->ImGuiCall();
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
