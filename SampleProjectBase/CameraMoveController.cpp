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

		// �X�e�[�g���쐬
		CreateState<CameraFollowMove>(Follow);
		CreateState<CameraChargeAttack>(Charge);
		CreateState<CameraWinState>(Win);
		CreateState<CameraGameOverState>(Lose);

		// �f�t�H���g��Ԃ��Z�b�g
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
		// �X�e�[�g����J�����̃x�[�X���W���X�V
		StateMachine_Base::Update();

		// �h��̑傫�����I�u�W�F�N�g���̍��W�ɔ��f
		ShakeUpdate();

		// �ŏI�I�ȃJ�����̍��W���X�V
		UpdateFinalPos();

		// ���t���[���̂��߂ɒǏ]��̍��W�擾
		if (GetHasFollowObject())
			prevFollowPos = pFollowTransform->GetPosition();
	}

	void CameraMoveController::ShakeUpdate()
	{
		// �p�[�����m�C�Y�ŃV�F�C�N����I�t�Z�b�g�l�����߂�
		perlinShake.Update(pCamera->DeltaTime());
	}

	void CameraMoveController::UpdateFinalPos()
	{
		Transform& camTransform = pCamera->GetTransform();

		// �J�����̌������l���������[���h��Ԃ̗h������߂�
		DXSimp::Vector3 worldShakeOffset;
		if (perlinShake.GetIsShaking())
		{
			const DXSimp::Vector3& shakeOffset = perlinShake.GetShakeOffset();

			worldShakeOffset = camTransform.Right() * shakeOffset.x +
				camTransform.Up() * shakeOffset.y +
				camTransform.Forward() * shakeOffset.z;

			HASHI_DEBUG_LOG("�����");
		}

		// ���W���Z�b�g
		camTransform.SetPosition(curBaseCameraPos + worldShakeOffset);
	}

	void CameraMoveController::ChangeState(CameraMoveState_Base::CameraState _cameraState, bool _isForce)
	{
		ChangeNode(_cameraState, _isForce);
	}

	void CameraMoveController::BeginShake(const PerlinShakeParameter& _shakeParam)
	{
		perlinShake.BeginShake(_shakeParam);

		HASHI_DEBUG_LOG("�J�n");
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

		// �e��Ԃ��Z�[�u
		auto& stateDatas = data["stateDatas"];
		for (auto& state : stateNodeList)
		{
			json stateData;

			CameraMoveState_Base& camState = CastCamState(*state.second);

			stateData["stateType"] = state.first;	// �X�e�[�g��
			stateData["parameter"] = camState.Save();	// �X�e�[�g�̃Z�[�u���
			stateDatas.push_back(stateData);	// �ǉ�
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
			HASHI_DEBUG_LOG("��ɒǏ]���ǉ����Ă�������");
			return;
		}

		DXSimp::Vector3 initPos;

		// �ʏ�ړ��J��������J�����̃g�����X�t�H�[����������
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

		// ���݂̃X�e�[�g��
		std::string curStateName = "current:" + GetStateName(currentStateKey);
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

	void CameraMoveController::LoadStates(const json& _data)
	{
		json stateDatas;
		// �X�e�[�g�̃f�[�^���Ȃ��Ȃ�
		if (!LoadJsonDataArray("stateDatas", stateDatas, _data)) return;

		// �e��Ԃ����[�h
		for (auto& stateData : stateDatas)
		{
			// �X�e�[�g�̃^�C�v�����[�h
			CameraMoveState_Base::CameraState loadState;
			if (!LoadJsonEnum<CameraMoveState_Base::CameraState>(
				"stateType", loadState, stateData
			)) continue;

			// �쐬����Ă��Ȃ��Ȃ�
			if (!stateNodeList.contains(loadState)) continue;

			// �p�����[�^�̃f�[�^���Ȃ��Ȃ�
			json paramData;
			if (!LoadJsonData("parameter", paramData, stateData))
				continue;

			// ���[�h����
			CastCamState(*stateNodeList[loadState]).Load(paramData);
		}
	}
}