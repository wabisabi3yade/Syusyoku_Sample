#include "pch.h"
#include "CameraMoveController.h"
#include "GameObject.h"

#include "CameraOnMoveState.h"

CameraMoveController::CameraMoveController()
	: StateMachine_Base("CameraMove")
{
	using enum CameraMoveState_Base::CameraState;

	// �X�e�[�g���쐬
	CreateState<CameraOnMoveState>(Move);

	// �f�t�H���g��Ԃ��Z�b�g
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

	// �e��Ԃ��Z�[�u
	auto& stateDatas = data["stateDatas"];
	for (auto& state : stateNodeList)
	{
		nlohmann::json stateData;

		CameraMoveState_Base& camState = CastCamState(*state.second);

		stateData["stateType"] = state.first;	// �X�e�[�g��
		stateData["parameter"] = camState.Save();	// �X�e�[�g�̃Z�[�u���
		stateDatas.push_back(stateData);	// �ǉ�
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
	// ���݂̃X�e�[�g��
	std::string curStateName = "current" + GetStateName(currentStateKey);
	ImGui::Text(curStateName.c_str());

	// �e�X�e�[�g�ҏW
	for (auto& state : stateNodeList)
	{
		// �X�e�[�g���擾
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
	// �X�e�[�g�̃f�[�^���Ȃ��Ȃ�
	if (!HashiTaku::LoadJsonDataArray("stateDatas", stateDatas, _data)) return;

	// �e��Ԃ����[�h
	for (auto& stateData : stateDatas)
	{
		// �X�e�[�g�̃^�C�v�����[�h
		CameraMoveState_Base::CameraState loadState;
		if (!HashiTaku::LoadJsonEnum<CameraMoveState_Base::CameraState>(
			"stateType", loadState, stateData
		)) continue;

		// �쐬����Ă��Ȃ��Ȃ�
		if (!stateNodeList.contains(loadState)) continue;

		// �p�����[�^�̃f�[�^���Ȃ��Ȃ�
		nlohmann::json paramData;
		if (!HashiTaku::LoadJsonData("parameter", paramData, stateData))
			continue;

		// ���[�h����
		CastCamState(*stateNodeList[loadState]).Load(paramData);
	}
}

