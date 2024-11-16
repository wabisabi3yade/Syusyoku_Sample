#include "pch.h"
#include "CameraMoveController.h"
#include "GameObject.h"

#include "CameraOnMoveState.h"

CameraMoveController::CameraMoveController()
	: StateMachine_Base("CameraMove")
{
	using enum CameraMoveState_Base::CameraState;

	// ステートを作成
	CreateState<CameraOnMoveState>(Move);

	// デフォルト状態をセット
	SetDefaultNode(Move);
}

void CameraMoveController::Begin(CP_Camera& _camera)
{
	pCamera = &_camera;

	StateMachine_Base::Begin();
}

void CameraMoveController::Update()
{
	StateMachine_Base::Update();
}

void CameraMoveController::ChangeState(CameraMoveState_Base::CameraState _cameraState, bool _isForce)
{
	ChangeNode(_cameraState, _isForce);
}

void CameraMoveController::SetTargetTransform(const Transform* _pTransform)
{
	pTargetTransform = _pTransform;
}

CP_Camera& CameraMoveController::GetCamera()
{
	return *pCamera;
}

const Transform* CameraMoveController::GetTargetTransform() const
{
	return pTargetTransform;
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

std::string CameraMoveController::GetStateName(CameraMoveState_Base::CameraState _state)
{
	return 	std::string(
		magic_enum::enum_name<CameraMoveState_Base::CameraState>(_state)
	);
}

CameraMoveState_Base& CameraMoveController::CastCamState(HashiTaku::StateNode_Base& _stateBase)
{
	return static_cast<CameraMoveState_Base&>(_stateBase);
}

void CameraMoveController::ImGuiDebug()
{
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

void CameraMoveController::LoadStates(const nlohmann::json& _data)
{
	nlohmann::json stateDatas;
	// ステートのデータがないなら
	if (!HashiTaku::LoadJsonDataArray("stateDatas", stateDatas, _data)) return;

	// 各状態をロード
	for (auto& stateData : stateDatas)
	{
		// ステートのタイプをロード
		CameraMoveState_Base::CameraState loadState;
		if (!HashiTaku::LoadJsonEnum<CameraMoveState_Base::CameraState>(
			"stateType", loadState, stateData
		)) continue;

		// 作成されていないなら
		if (!stateNodeList.contains(loadState)) continue;

		// パラメータのデータがないなら
		nlohmann::json paramData;
		if (!HashiTaku::LoadJsonData("parameter", paramData, stateData))
			continue;

		// ロードする
		CastCamState(*stateNodeList[loadState]).Load(paramData);
	}
}

