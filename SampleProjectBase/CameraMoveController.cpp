#include "pch.h"
#include "CameraMoveController.h"
#include "GameObject.h"

#include "CameraOnMoveState.h"
#include "CameraGameOverState.h"

namespace HashiTaku
{
	constexpr float MAX_RAND_PERLINOFFSET(50.0f); // �p�[�����m�C�Y�̃I�t�Z�b�g�l�̍ő�l

	CameraMoveController::CameraMoveController() :
		StateMachine_Base("CameraMove"),
		pFollowTransform(nullptr),
		pLookAtTransform(nullptr),
		shakeElapsedTime(0.0f),
		isShaking(false)
	{
		using enum CameraState;

		// �X�e�[�g���쐬
		CreateState<CameraOnMoveState>(Move);
		CreateState<CameraGameOverState>(Win);
		CreateState<CameraGameOverState>(Lose);

		// �f�t�H���g��Ԃ��Z�b�g
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
		if (!isShaking) return;

		/// �o�ߎ��Ԃ����Z
		shakeElapsedTime += GetCamera().DeltaTime();

		// �I�t�Z�b�g�l�����߂�
		CalcShakeOffset();

		// �S�̎��Ԃ��߂�����I���
		if (shakeElapsedTime > curShakeParameter.time)
			OnEndShake();
	}

	void CameraMoveController::CalcShakeOffset()
	{
		float speed = curShakeParameter.speed;

		// ���Ԍo�߂Ńt�F�[�h�A�E�g������t�F�[�h�A�E�g���������߂�
		float fadeOutRatio = 1.0f;
		if (curShakeParameter.isFadeOut)
		{
			fadeOutRatio -= std::clamp(shakeElapsedTime / curShakeParameter.time, 0.0f, 1.0f);
		}

		// X��
		if (curShakeParameter.shakeVec.x > Mathf::epsilon)
		{
			// �p�[�����m�C�Y����擾
			curShakeOffsetPos.x = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.x * speed * shakeElapsedTime
				)));
			curShakeOffsetPos.x -= 0.5f;
			curShakeOffsetPos.x *= 2.0f;
			
			HASHI_DEBUG_LOG(std::to_string(curShakeOffsetPos.x));

			// �h��̗͂̔{�������߂�
			curShakeOffsetPos.x *= curShakeParameter.power * fadeOutRatio;

			// �x�N�g���̑傫�����|����
			curShakeOffsetPos.x *= curShakeParameter.shakeVec.x;
		}
		// Y��
		if (curShakeParameter.shakeVec.y > Mathf::epsilon)
		{
			// �p�[�����m�C�Y����擾
			curShakeOffsetPos.y = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.y * speed * shakeElapsedTime
				)));
			curShakeOffsetPos.y -= 0.5f;
			curShakeOffsetPos.y *= 2.0f;

			// �h��̗͂̔{�������߂�
			curShakeOffsetPos.y *= curShakeParameter.power * fadeOutRatio;

			// �x�N�g���̑傫�����|����
			curShakeOffsetPos.y *= curShakeParameter.shakeVec.y;
		}
		// Z��
		if (curShakeParameter.shakeVec.z > Mathf::epsilon)
		{
			// �p�[�����m�C�Y����擾
			curShakeOffsetPos.z= static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.z * speed * shakeElapsedTime
				)));
			curShakeOffsetPos.z -= 0.5f;
			curShakeOffsetPos.z *= 2.0f;

			// �h��̗͂̔{�������߂�
			curShakeOffsetPos.z *= curShakeParameter.power * fadeOutRatio;

			// �x�N�g���̑傫�����|����
			curShakeOffsetPos.z *= curShakeParameter.shakeVec.z;
		}
	}

	void CameraMoveController::UpdateFinalPos()
	{
		Transform& camTransform = pCamera->GetTransform();

		// �J�����̌������l���������[���h��Ԃ̗h������߂�
		DXSimp::Vector3 worldShakeOffset;
		if (isShaking)
		{
			worldShakeOffset = camTransform.Right() * curShakeOffsetPos.x +
				camTransform.Up() * curShakeOffsetPos.y +
				camTransform.Forward() * curShakeOffsetPos.z;
		}

		// ���W���Z�b�g
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

		// �����Ńp�[�����m�C�Y�̃I�t�Z�b�g�l�����߂�
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

	void CameraMoveController::InitCameraPos()
	{
		if (!GetHasFollowObject())
		{
			HASHI_DEBUG_LOG("��ɒǏ]���ǉ����Ă�������");
			return;
		}

		DXSimp::Vector3 initPos;

		// �ʏ�ړ��J��������J�����̃g�����X�t�H�[����������
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
			nlohmann::json paramData;
			if (!LoadJsonData("parameter", paramData, stateData))
				continue;

			// ���[�h����
			CastCamState(*stateNodeList[loadState]).Load(paramData);
		}
	}
}