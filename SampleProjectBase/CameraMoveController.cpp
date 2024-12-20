#include "pch.h"
#include "CameraMoveController.h"
#include "GameObject.h"

#include "CameraOnMoveState.h"
#include "CameraGameOverState.h"

namespace HashiTaku
{
	constexpr float MAX_RAND_PERLINOFFSET(50.0f); // パーリンノイズのオフセット値の最大値

	CameraMoveController::CameraMoveController() :
		StateMachine_Base("CameraMove"),
		pFollowTransform(nullptr),
		pLookAtTransform(nullptr),
		shakeElapsedTime(0.0f),
		isShaking(false)
	{
		using enum CameraState;

		// ステートを作成
		CreateState<CameraOnMoveState>(Move);
		CreateState<CameraGameOverState>(Win);
		CreateState<CameraGameOverState>(Lose);

		// デフォルト状態をセット
		SetDefaultNode(Move);
	}

	void CameraMoveController::Begin(CP_Camera& _camera)
	{
		pCamera = &_camera;

		InitCameraPos();

		StateMachine_Base::Begin();
	}

	void CameraMoveController::Update()
	{
		// ステートからカメラのベース座標を更新
		StateMachine_Base::Update();

		// 揺れの大きさをオブジェクト側の座標に反映
		ShakeUpdate();

		// 最終的なカメラの座標を更新
		UpdateFinalPos();

		// 次フレームのために追従先の座標取得
		if (GetHasFollowObject())
			prevFollowPos = pFollowTransform->GetPosition();
	}

	void CameraMoveController::ShakeUpdate()
	{
		if (!isShaking) return;

		/// 経過時間を加算
		shakeElapsedTime += GetCamera().DeltaTime();

		// オフセット値を求める
		CalcShakeOffset();

		// 全体時間を過ぎたら終わる
		if (shakeElapsedTime > curShakeParameter.time)
			OnEndShake();
	}

	void CameraMoveController::CalcShakeOffset()
	{
		float speed = curShakeParameter.speed;

		// 時間経過でフェードアウトさせるフェードアウト割合を求める
		float fadeOutRatio = 1.0f;
		if (curShakeParameter.isFadeOut)
		{
			fadeOutRatio -= std::clamp(shakeElapsedTime / curShakeParameter.time, 0.0f, 1.0f);
		}

		// X軸
		if (curShakeParameter.shakeVec.x > Mathf::epsilon)
		{
			// パーリンノイズから取得
			curShakeOffsetPos.x = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.x * speed * shakeElapsedTime
				)));
			curShakeOffsetPos.x -= 0.5f;
			curShakeOffsetPos.x *= 2.0f;
			
			HASHI_DEBUG_LOG(std::to_string(curShakeOffsetPos.x));

			// 揺れの力の倍率を求める
			curShakeOffsetPos.x *= curShakeParameter.power * fadeOutRatio;

			// ベクトルの大きさも掛ける
			curShakeOffsetPos.x *= curShakeParameter.shakeVec.x;
		}
		// Y軸
		if (curShakeParameter.shakeVec.y > Mathf::epsilon)
		{
			// パーリンノイズから取得
			curShakeOffsetPos.y = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.y * speed * shakeElapsedTime
				)));
			curShakeOffsetPos.y -= 0.5f;
			curShakeOffsetPos.y *= 2.0f;

			// 揺れの力の倍率を求める
			curShakeOffsetPos.y *= curShakeParameter.power * fadeOutRatio;

			// ベクトルの大きさも掛ける
			curShakeOffsetPos.y *= curShakeParameter.shakeVec.y;
		}
		// Z軸
		if (curShakeParameter.shakeVec.z > Mathf::epsilon)
		{
			// パーリンノイズから取得
			curShakeOffsetPos.z= static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.z * speed * shakeElapsedTime
				)));
			curShakeOffsetPos.z -= 0.5f;
			curShakeOffsetPos.z *= 2.0f;

			// 揺れの力の倍率を求める
			curShakeOffsetPos.z *= curShakeParameter.power * fadeOutRatio;

			// ベクトルの大きさも掛ける
			curShakeOffsetPos.z *= curShakeParameter.shakeVec.z;
		}
	}

	void CameraMoveController::UpdateFinalPos()
	{
		Transform& camTransform = pCamera->GetTransform();

		// カメラの向きを考慮したワールド空間の揺れを求める
		DXSimp::Vector3 worldShakeOffset;
		if (isShaking)
		{
			worldShakeOffset = camTransform.Right() * curShakeOffsetPos.x +
				camTransform.Up() * curShakeOffsetPos.y +
				camTransform.Forward() * curShakeOffsetPos.z;
		}

		// 座標をセット
		camTransform.SetPosition(curBaseCameraPos + worldShakeOffset);
	}

	void CameraMoveController::ChangeState(CameraMoveState_Base::CameraState _cameraState, bool _isForce)
	{
		ChangeNode(_cameraState, _isForce);
	}

	void CameraMoveController::SetFollowTransform(const Transform* _pTransform)
	{
		pFollowTransform = _pTransform;
	}

	void CameraMoveController::SetLookAtObject(const ITargetAccepter* _pTransform)
	{
		pLookAtTransform = _pTransform;
	}

	void CameraMoveController::SetCameraBasePos(const DirectX::SimpleMath::Vector3& _camPos)
	{
		curBaseCameraPos = _camPos;
	}

	void CameraMoveController::SetFov(float _setDegree)
	{
		pCamera->SetFov(_setDegree);
	}

	void CameraMoveController::BeginShake(const DXSimp::Vector3& _vector, 
		float _power, 
		float _time, 
		float _speed,
		bool _isFadeOut)
	{
		isShaking = true;
		shakeElapsedTime = 0.0f;

		curShakeParameter.isFadeOut = _isFadeOut;
		curShakeParameter.power = _power;
		curShakeParameter.time = _time;
		curShakeParameter.speed = _speed;
		curShakeParameter.shakeVec = _vector;

		// 乱数でパーリンノイズのオフセット値を決める
		randPerlinOffset.x = Random::Range<float>(0.0f, MAX_RAND_PERLINOFFSET);
		randPerlinOffset.y = Random::Range<float>(0.0f, MAX_RAND_PERLINOFFSET);
		randPerlinOffset.z = Random::Range<float>(0.0f, MAX_RAND_PERLINOFFSET);
	}

	void CameraMoveController::BeginShake(const CameraShakeParameter& _shakeParam)
	{
		BeginShake(_shakeParam.shakeVec,
			_shakeParam.power,
			_shakeParam.time,
			_shakeParam.speed,
			_shakeParam.isFadeOut);
	}

	void CameraMoveController::OnPlayerWin(const Transform& _targetTransform)
	{
		StateNode_Base* pState = GetNode(CameraState::Win);
		if (!pState) return;

		CameraGameOverState* pGameOver = static_cast<CameraGameOverState*>(pState);
		pGameOver->SetTargetTransform(_targetTransform);

		ChangeState(CameraState::Win, true);
	}

	float CameraMoveController::GetFov() const
	{
#ifdef EDIT
		if (!pCamera) return 0.0f;
#endif // EDIT

		return pCamera->GetFov();
	}

	CP_Camera& CameraMoveController::GetCamera()
	{
		return *pCamera;
	}

	const DirectX::SimpleMath::Vector3& CameraMoveController::GetCameraBasePos()
	{
		return curBaseCameraPos;
	}

	const DirectX::SimpleMath::Vector3& CameraMoveController::GetPrevFollowPos() const
	{
		return prevFollowPos;
	}

	bool CameraMoveController::GetHasFollowObject() const
	{
		return pFollowTransform != nullptr;
	}

	const Transform* CameraMoveController::GetFollowTransform() const
	{
		return pFollowTransform;
	}

	bool CameraMoveController::GetHaveTarget() const
	{
		return pFollowTransform != nullptr;
	}

	const DirectX::SimpleMath::Vector3& CameraMoveController::GetLookAtWorldPos() const
	{
		if (!pLookAtTransform) return DirectX::SimpleMath::Vector3::Zero;

		return pLookAtTransform->GetWorldPos();
	}

	nlohmann::json CameraMoveController::Save()
	{
		nlohmann::json data;

		// 各状態をセーブ
		auto& stateDatas = data["stateDatas"];
		for (auto& state : stateNodeList)
		{
			nlohmann::json stateData;

			CameraMoveState_Base& camState = CastCamState(*state.second);

			stateData["stateType"] = state.first;	// ステート名
			stateData["parameter"] = camState.Save();	// ステートのセーブ情報
			stateDatas.push_back(stateData);	// 追加
		}

		return data;
	}

	void CameraMoveController::Load(const nlohmann::json& _data)
	{
		LoadStates(_data);
	}

	void CameraMoveController::InitCameraPos()
	{
		if (!GetHasFollowObject())
		{
			HASHI_DEBUG_LOG("先に追従先を追加してください");
			return;
		}

		DXSimp::Vector3 initPos;

		// 通常移動カメラからカメラのトランスフォームを初期化
		auto& stateNode = stateNodeList[CameraMoveState_Base::CameraState::Move];
		if (auto moveState = dynamic_cast<CameraOnMoveState*>(stateNode.get()))
		{
			moveState->InitCameraTransform();
		}
	}

	void CameraMoveController::OnEndShake()
	{
		isShaking = false;
		curShakeOffsetPos = DXSimp::Vector3::Zero;
	}

	std::string CameraMoveController::GetStateName(CameraMoveState_Base::CameraState _state)
	{
		return 	std::string(
			magic_enum::enum_name<CameraMoveState_Base::CameraState>(_state)
		);
	}

	CameraMoveState_Base& CameraMoveController::CastCamState(StateNode_Base& _stateBase)
	{
		return static_cast<CameraMoveState_Base&>(_stateBase);
	}

	void CameraMoveController::ImGuiDebug()
	{
		ImGuiShakeDebug();

		// 現在のステート名
		std::string curStateName = "current" + GetStateName(currentStateKey);
		ImGui::Text(curStateName.c_str());

		// 各ステート編集
		for (auto& state : stateNodeList)
		{
			// ステート名取得
			std::string stateName = GetStateName(state.first);

			if (ImGuiMethod::TreeNode(stateName))
			{
				CameraMoveState_Base& camState =
					static_cast<CameraMoveState_Base&>(*state.second);
				camState.ImGuiCall();
				ImGui::TreePop();
			}
		}
	}

	void CameraMoveController::ImGuiShakeDebug()
	{
#ifdef EDIT
		if (ImGui::Button("Test Shake"))
			BeginShake(debugShakeParam);

		debugShakeParam.ImGuiCall();

		if (!isShaking) return;
#endif // EDIT
	}

	void CameraMoveController::LoadStates(const nlohmann::json& _data)
	{
		nlohmann::json stateDatas;
		// ステートのデータがないなら
		if (!LoadJsonDataArray("stateDatas", stateDatas, _data)) return;

		// 各状態をロード
		for (auto& stateData : stateDatas)
		{
			// ステートのタイプをロード
			CameraMoveState_Base::CameraState loadState;
			if (!LoadJsonEnum<CameraMoveState_Base::CameraState>(
				"stateType", loadState, stateData
			)) continue;

			// 作成されていないなら
			if (!stateNodeList.contains(loadState)) continue;

			// パラメータのデータがないなら
			nlohmann::json paramData;
			if (!LoadJsonData("parameter", paramData, stateData))
				continue;

			// ロードする
			CastCamState(*stateNodeList[loadState]).Load(paramData);
		}
	}
}