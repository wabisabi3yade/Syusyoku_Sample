#include "pch.h"
#include "CameraMoveController.h"
#include "GameObject.h"

#include "CameraFollowMove.h"
#include "CameraChargeAttack.h"
#include "CameraWinState.h"
#include "CameraGameOverState.h"

namespace HashiTaku
{
	CameraMoveController::CameraMoveController() :
		StateMachine_Base("CameraMove"),
		pFollowTransform(nullptr),
		pLookAtTransform(nullptr)
	{
		using enum CameraState;

		// ステートを作成
		CreateState<CameraFollowMove>(Follow);
		CreateState<CameraChargeAttack>(Charge);
		CreateState<CameraWinState>(Win);
		CreateState<CameraGameOverState>(Lose);

		// デフォルト状態をセット
		SetDefaultNode(Follow);
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
		// パーリンノイズでシェイクするオフセット値を求める
		perlinShake.Update(pCamera->DeltaTime());
	}

	void CameraMoveController::UpdateFinalPos()
	{
		Transform& camTransform = pCamera->GetTransform();

		// カメラの向きを考慮したワールド空間の揺れを求める
		DXSimp::Vector3 worldShakeOffset;
		if (perlinShake.GetIsShaking())
		{
			const DXSimp::Vector3& shakeOffset = perlinShake.GetShakeOffset();

			worldShakeOffset = camTransform.Right() * shakeOffset.x +
				camTransform.Up() * shakeOffset.y +
				camTransform.Forward() * shakeOffset.z;

			HASHI_DEBUG_LOG("ゆれゆれ");
		}

		// 座標をセット
		camTransform.SetPosition(curBaseCameraPos + worldShakeOffset);
	}

	void CameraMoveController::ChangeState(CameraMoveState_Base::CameraState _cameraState, bool _isForce)
	{
		ChangeNode(_cameraState, _isForce);
	}

	void CameraMoveController::BeginShake(const PerlinShakeParameter& _shakeParam)
	{
		perlinShake.BeginShake(_shakeParam);

		HASHI_DEBUG_LOG("開始");
	}

	void CameraMoveController::StopShake()
	{
		perlinShake.EndShake();
	}

	void CameraMoveController::SetFollowTransform(const Transform* _pTransform)
	{
		pFollowTransform = _pTransform;
	}

	void CameraMoveController::SetLookAtObject(const ITargetAccepter* _pTransform)
	{
		pLookAtTransform = _pTransform;
	}

	void CameraMoveController::SetCameraBasePos(const DXSimp::Vector3& _camPos)
	{
		curBaseCameraPos = _camPos;
	}

	void CameraMoveController::SetFov(float _setDegree)
	{
#ifdef EDIT
		if (!pCamera) return;
#endif // EDIT

		pCamera->SetFov(_setDegree);
	}

	void CameraMoveController::OnPlayerWin(const Transform& _targetTransform)
	{
		StateNode_Base* pState = GetNode(CameraState::Win);
		if (!pState) return;

		CameraWinState* pWinState = static_cast<CameraWinState*>(pState);
		pWinState->SetTargetTransform(_targetTransform);

		ChangeState(CameraState::Win, true);
	}

	void CameraMoveController::OnPlayerLose()
	{
		ChangeState(CameraState::Lose, true);
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

	const DXSimp::Vector3& CameraMoveController::GetCameraBasePos()
	{
		return curBaseCameraPos;
	}

	const DXSimp::Vector3& CameraMoveController::GetPrevFollowPos() const
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

	const DXSimp::Vector3& CameraMoveController::GetLookAtWorldPos() const
	{
		if (!pLookAtTransform) return DXSimp::Vector3::Zero;

		return pLookAtTransform->GetWorldPos();
	}

	json CameraMoveController::Save()
	{
		json data;

		// 各状態をセーブ
		auto& stateDatas = data["stateDatas"];
		for (auto& state : stateNodeList)
		{
			json stateData;

			CameraMoveState_Base& camState = CastCamState(*state.second);

			stateData["stateType"] = state.first;	// ステート名
			stateData["parameter"] = camState.Save();	// ステートのセーブ情報
			stateDatas.push_back(stateData);	// 追加
		}

		return data;
	}

	void CameraMoveController::Load(const json& _data)
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
		auto& stateNode = stateNodeList[CameraMoveState_Base::CameraState::Follow];
		if (auto moveState = dynamic_cast<CameraFollowMove*>(stateNode.get()))
		{
			moveState->InitCameraTransform();
		}
	}

	std::string CameraMoveController::GetStateName(CameraMoveState_Base::CameraState _state)
	{
		return 	std::string(magic_enum::enum_name(_state));
	}

	CameraMoveState_Base& CameraMoveController::CastCamState(StateNode_Base& _stateBase)
	{
		return static_cast<CameraMoveState_Base&>(_stateBase);
	}

	void CameraMoveController::ImGuiDebug()
	{
		//ImGuiShakeDebug();

		// 現在のステート名
		std::string curStateName = "current:" + GetStateName(currentStateKey);
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

	void CameraMoveController::LoadStates(const json& _data)
	{
		json stateDatas;
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
			json paramData;
			if (!LoadJsonData("parameter", paramData, stateData))
				continue;

			// ロードする
			CastCamState(*stateNodeList[loadState]).Load(paramData);
		}
	}
}