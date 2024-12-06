#include "pch.h"
#include "PlayerAction.h"
#include "InSceneSystemManager.h"

// アクションできる左スティックの入力量
constexpr float CAN_ACTION_STICKINPUT(0.7f);

// アニメーションコントローラ内のプレイヤー名
constexpr  auto STATEANIM_PARAMNAME{ "state" };
// ターゲット中を表すアニメーションパラメータ
constexpr auto TARGET_PARAMNAME{ "targeting" };
// キャンセルできるかを表すアニメーションパラメータ
constexpr auto CANCEL_PARAMNAME{ "canCancel" };
// 先行入力できるかを表すアニメーションパラメータ
constexpr auto INPUT_PARAMNAME{ "canInput" };
// 攻撃できるかを表すアニメーションパラメータ
constexpr auto CANATK_PARAMNAME{ "canAttack" };
// 移動できるかを表すアニメーションパラメータ
constexpr auto CANMOVE_PARAMNAME{ "canMove" };

PlayerAction::PlayerAction()
{
	InSceneSystemManager* pInsceneSysytem = InSceneSystemManager::GetInstance();
	pInput = &pInsceneSysytem->GetInput();

	pGroundContoroller = std::make_unique<PlayerActionController>();

	pAirContoroller = std::make_unique<PlayerActionController>();
}

PlayerAction::~PlayerAction()
{
	// ターゲットする側に削除するように通知
	if (pTargetObject)
		pTargetObject->RemoveTargeter(*this);
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
	// アニメーションパラメータのアドレスを取得
	pIsCanCancel = pAnimation->GetParameterPointer<bool>(CANCEL_PARAMNAME);
	// 先行入力のアドレスを取得
	pIsCanInput = pAnimation->GetParameterPointer<bool>(INPUT_PARAMNAME);
	// 攻撃可能アドレスを取得
	pIsCanAttack = pAnimation->GetParameterPointer<bool>(CANATK_PARAMNAME);
	// 移動可能アドレスを取得
	pIsCanMove = pAnimation->GetParameterPointer<bool>(CANMOVE_PARAMNAME);

	pGroundContoroller->Init(pAnimation,_pRigidBody);

	pAirContoroller->Init(pAnimation, _pRigidBody);
}

void PlayerAction::Update()
{
}

CP_Camera& PlayerAction::GetCamera()
{
	return *pCamera;
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

bool PlayerAction::GetIsCanCancel() const
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

nlohmann::json PlayerAction::Save()
{
	nlohmann::json data;

	data["ground"] = pGroundContoroller->Save();
	data["air"] = pAirContoroller->Save();

	return data;
}

void PlayerAction::Load(const nlohmann::json& _data)
{
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
	if (!pTargetObject) return;

	if (pTargetCamera)	// カメラにも伝える
	{
		pTargetCamera->RemoveNotify(*pTargetObject);
		pTargetObject->RemoveTargeter(*pTargetCamera);
	}

	// ターゲットで参照するのをやめる
	pTargetObject->RemoveTargeter(*this);
	pTargetObject = nullptr;
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
