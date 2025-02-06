#include "pch.h"
#include "PlayerActState_Base.h"
#include "PlayerActionController_Base.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	GameInput* PlayerActState_Base::pPlayerInput = nullptr;

	// アクションできる左スティックの入力量
	constexpr float CAN_ACTION_STICKINPUT(0.7f);
	// スティックの方向の内積でこれ以上一致していないなら反応しない
	constexpr float INPUT_VECTOR_DOT(0.6f);
	// 移動でキャンセルができるスティックの入力量
	constexpr float CAN_MOVECANCEL_INPUT(0.3f);
	// キャンセル入力で予約した状態の有効期限時間（超えると予約した状態はリセットされる）
	constexpr float CANCEL_RESERVE_VALIED_TIME(0.3f);

	namespace DXSimp = DXSimp;

	PlayerActState_Base::PlayerActState_Base() :
		cancelType(CancelType::None),
		statePriority(-1),
		targetLookRotateSpeed(40.0f),
		isTargetLookAtEnemy(false)
	{
		pPlayerInput = &InSceneSystemManager::GetInstance()->GetInput();
	}

	void PlayerActState_Base::Init(PlayerActionController_Base& _actController, 
		CancelType _cancelType,
		int _priority)
	{
		CharacterActState_Base::Init(_actController);
		cancelType = _cancelType;
		statePriority = _priority;

		// ステートの初期化
		InitState();
	}

	void PlayerActState_Base::Update()
	{
		CharacterActState_Base::Update();

		// ターゲットの方向を見る
		UpdateTargetLook();
	}

	PlayerActState_Base::CancelType PlayerActState_Base::GetCancelType() const
	{
		return cancelType;
	}

	int PlayerActState_Base::GetPriority() const
	{
		return statePriority;
	}

	json PlayerActState_Base::Save()
	{
		return json();
	}

	void PlayerActState_Base::Load(const json& _data)
	{
	}

	void PlayerActState_Base::ClearVelocity(bool _applyY)
	{
		DXSimp::Vector3 set;
		IActionController& actCon = GetActionController();
		if (!_applyY) // y軸反映させたくないなら
		{
			set.y = actCon.GetVelocity().y;
		}
		actCon.SetVelocity(set);
	}

	void PlayerActState_Base::SetTargetAtEnemy(bool _isLook)
	{
		isTargetLookAtEnemy = _isLook;
	}

	void PlayerActState_Base::SetInvicible(bool _isInvicible)
	{
		GetPlayer().SetIsInvicible(_isInvicible);
	}

	CP_Animation* PlayerActState_Base::GetAnimation()
	{
		return GetActionController().GetAnimation();
	}

	const ITargetAccepter* PlayerActState_Base::GetTargetAccepter()
	{
		return GetPlayerActionController().GetTargetAccepter();
	}

	PlayerActState_Base* PlayerActState_Base::GetPlayerState(int _getStateId)
	{
		return GetPlayerActionController().GetPlayerState(_getStateId);
	}

	CP_Player& PlayerActState_Base::GetPlayer()
	{
		return GetPlayerActionController().GetPlayer();
	}

	float PlayerActState_Base::GetDeltaSpeed()
	{
		return GetActionController().GetCharacter().GetDeltaSpeed();
	}

	DXSimp::Vector2 PlayerActState_Base::GetInputLeftStick() const
	{
		return pPlayerInput->GetValue(GameInput::ValueType::Player_Move);
	}

	DXSimp::Vector2 PlayerActState_Base::GetInputLeftStickFromCam()
	{
		// 入力値を取得
		DXSimp::Vector2 inputVec = GetInputLeftStick();

		// カメラから見た入力とする
		const Transform& camTrans = GetPlayerActionController().GetCamera().GetTransform();
		DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
			inputVec.y * camTrans.Forward();
		inputVec = { inputVecByCam.x, inputVecByCam.z };
		inputVec.Normalize();

		return inputVec;
	}

	DXSimp::Vector3 PlayerActState_Base::GetAtkEnemyPos()
	{
		PlayerActionController_Base& actCon = GetPlayerActionController();

		// ターゲットしているなら
		if (actCon.GetIsTargeting())
		{
			auto* pAccepter = actCon .GetTargetAccepter();
			if (pAccepter)
				return pAccepter->GetWorldPos();
		}

		// 近くの敵を探す
		CP_BattleManager* pBattle = CP_BattleManager::GetInstance();
		if (!pBattle) return DXSimp::Vector3::Zero;

		// 敵リストを取得する
		const auto& enemyList = pBattle->GetEnemyList();

		// 敵がいないなら
		if (static_cast<u_int>(enemyList.size()) == 0) return DXSimp::Vector3::Zero;

		// 敵の攻撃座標
		DXSimp::Vector3 atkEnemyPos;
		const ITargetAccepter* pAtkEnemy = (*enemyList.begin());

		// 本来は一番近くの敵を取得する
		atkEnemyPos = pAtkEnemy->GetWorldPos();

		return atkEnemyPos;
	}

	bool PlayerActState_Base::IsInputVector(InputVector _checkVector)
	{
		// 入力値を取得
		DXSimp::Vector2 inputVec = GetInputLeftStick();
		if (inputVec.Length() < CAN_ACTION_STICKINPUT) return false;	// 傾きが十分でないなら

		PlayerActionController_Base& actCon = GetPlayerActionController();

		// カメラから見た入力とする
		const Transform& camTrans = actCon.GetCamera().GetTransform();
		DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
			inputVec.y * camTrans.Forward();
		inputVec = { inputVecByCam.x, inputVecByCam.z };
		inputVec.Normalize();

		// 向きを取得
		DXSimp::Vector3 baseVec;

		// ターゲット時で敵がいるなら　敵の方向ベクトルを基準ベクトルに
		if (actCon.GetIsTargeting() &&
			GetTargetAccepter())
		{
			DXSimp::Vector3 targetObjVec =
				GetTargetAccepter()->GetWorldPos() -
				GetMyTransform().GetPosition();

			targetObjVec.Normalize(baseVec);
		}
		else // 違うなら　プレイヤーの前ベクトル
			baseVec = GetPlayer().GetTransform().Forward();

		DXSimp::Vector2 baseVecXZ = { baseVec.x, baseVec.z };
		baseVecXZ.Normalize();

		// スティックの方向が一致しているか見る
		float dot = inputVec.Dot(baseVecXZ);

		return dot > INPUT_VECTOR_DOT ? true : false;
	}

	bool PlayerActState_Base::IsRollingInput() const
	{
		// ボタン入力
		if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Rolling)) return false;

		// ローリングできる左スティックの入力ができていない
		if (std::min(GetInputLeftStick().Length(), 1.0f) < CAN_ACTION_STICKINPUT) return false;

		return true;
	}

	bool PlayerActState_Base::IsSpecialAtkInput(InputVector _inputVecter)
	{
		// ボタン入力
		if (!pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack)) return false;

		// ターゲットしていないなら
		if (!GetPlayerActionController().GetIsTargeting()) return false;

		if (!IsInputVector(_inputVecter)) return false;

		return true;
	}

	void PlayerActState_Base::UpdateTargetLook()
	{
		// ターゲットの方向見ないなら
		if (!isTargetLookAtEnemy) return;

		PlayerActionController_Base& actCon = GetPlayerActionController();
		const ITargetAccepter* pTargetObj = actCon.GetTargetAccepter();
		if (!pTargetObj) return;	// ターゲットがいないなら

		Transform& transform = actCon.GetMyTransform();

		// ターゲットへのベクトルを求める
		const DXSimp::Vector3& playerPos = transform.GetPosition();
		const DXSimp::Vector3& targetPos = pTargetObj->GetWorldPos();
		DXSimp::Vector3 vector = targetPos - playerPos;
		vector.y = 0.0f; vector.Normalize();

		// 回転させる
		DXSimp::Quaternion targetRot = Quat::RotateToVector(vector);
		DXSimp::Quaternion myRot = transform.GetRotation();
		myRot = DXSimp::Quaternion::Slerp(myRot, 
			targetRot, 
			targetLookRotateSpeed * DeltaTime());

		transform.SetRotation(myRot);
	}
	PlayerActionController_Base& PlayerActState_Base::GetPlayerActionController()
	{
		return GetDeliverActionController<PlayerActionController_Base>();
	}
}