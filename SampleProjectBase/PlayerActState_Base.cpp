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

	namespace DXSimp = DirectX::SimpleMath;

	PlayerActState_Base::PlayerActState_Base() :
		pActionController(nullptr),
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
		pActionController = &_actController;
		cancelType = _cancelType;
		statePriority = _priority;
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
		if (!_applyY) // y軸反映させたくないなら
		{
			set.y = GetRB().GetVelocity().y;
		}
		GetRB().SetVelocity(set);
	}

	void PlayerActState_Base::SetTargetAtEnemy(bool _isLook)
	{
		isTargetLookAtEnemy = _isLook;
	}

	void PlayerActState_Base::SetInvicible(bool _isInvicible)
	{
		GetPlayer().SetIsInvicible(_isInvicible);
	}

	CP_RigidBody& PlayerActState_Base::GetRB()
	{
		return *pActionController->GetRB();
	}

	Transform& PlayerActState_Base::GetMyTransform()
	{
		return pActionController->GetMyTransform();
	}

	CP_Animation* PlayerActState_Base::GetAnimation()
	{
		return pActionController->GetAnimation();
	}

	const ITargetAccepter* PlayerActState_Base::GetTargetAccepter()
	{
		return pActionController->GetTargetAccepter();
	}

	CP_Player& PlayerActState_Base::GetPlayer()
	{
		return pActionController->GetPlayer();
	}

	float PlayerActState_Base::DeltaTime() const
	{
		return pActionController->GetCharacter().DeltaTime();
	}

	float PlayerActState_Base::GetDeltaSpeed() const
	{
		return pActionController->GetCharacter().GetDeltaSpeed();
	}

	DirectX::SimpleMath::Vector2 PlayerActState_Base::GetInputLeftStick() const
	{
		return pPlayerInput->GetValue(GameInput::ValueType::Player_Move);
	}

	DirectX::SimpleMath::Vector2 PlayerActState_Base::GetInputLeftStickFromCam() const
	{
		// 入力値を取得
		DXSimp::Vector2 inputVec = GetInputLeftStick();

		// カメラから見た入力とする
		const Transform& camTrans = pActionController->GetCamera().GetTransform();
		DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
			inputVec.y * camTrans.Forward();
		inputVec = { inputVecByCam.x, inputVecByCam.z };
		inputVec.Normalize();

		return inputVec;
	}

	DirectX::SimpleMath::Vector3 PlayerActState_Base::GetAtkEnemyPos()
	{
		// ターゲットしているなら
		if (pActionController->GetIsTargeting())
		{
			auto* pAccepter = pActionController->GetTargetAccepter();
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

		// カメラから見た入力とする
		const Transform& camTrans = pActionController->GetCamera().GetTransform();
		DXSimp::Vector3 inputVecByCam = inputVec.x * camTrans.Right() +
			inputVec.y * camTrans.Forward();
		inputVec = { inputVecByCam.x, inputVecByCam.z };
		inputVec.Normalize();

		// 向きを取得
		DXSimp::Vector3 baseVec;

		// ターゲット時で敵がいるなら　敵の方向ベクトルを基準ベクトルに
		if (pActionController->GetIsTargeting() &&
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
		if (!pActionController->GetIsTargeting()) return false;

		if (!IsInputVector(_inputVecter)) return false;

		return true;
	}

	void PlayerActState_Base::UpdateTargetLook()
	{
		// ターゲットの方向見ないなら
		if (!isTargetLookAtEnemy) return;

		const ITargetAccepter* pTargetObj = pActionController->GetTargetAccepter();
		if (!pTargetObj) return;	// ターゲットがいないなら

		Transform& transform = pActionController->GetMyTransform();

		// ターゲットへのベクトルを求める
		const DXSimp::Vector3& playerPos = transform.GetPosition();
		const DXSimp::Vector3& targetPos = pTargetObj->GetWorldPos();
		DXSimp::Vector3 vector = targetPos - playerPos;
		vector.y = 0.0f; vector.Normalize();

		// 回転させる
		DXSimp::Quaternion targetRot = Quat::RotateToVector(vector);
		DXSimp::Quaternion myRot = transform.GetRotation();
		myRot = DXSimp::Quaternion::Slerp(myRot, targetRot, targetLookRotateSpeed * DeltaTime());

		transform.SetRotation(myRot);
	}
}