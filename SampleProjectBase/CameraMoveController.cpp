#include "pch.h"
#include "CameraMoveController.h"
#include "GameObject.h"

#include "CameraOnMoveState.h"
#include "CameraTargetMove.h"

namespace DXSimp = DirectX::SimpleMath;

// �O��̗h��̎��Ԃ̏����l
constexpr float INIT_PREV_SHAKEELAPSETIME(-10000.0f);

CameraMoveController::CameraMoveController() :
	StateMachine_Base("CameraMove"), pTargetTransform(nullptr), pLookAtTransform(nullptr),
	shakeTime(0.0f), shakeElapsedTime(0.0f), prevShakeElapsedTime(0.0f), isShaking(false)
{
	using enum CameraMoveState_Base::CameraState;

	// �X�e�[�g���쐬
	CreateState<CameraOnMoveState>(Move);
	CreateState<CameraTargetMove>(Target);

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
	// �X�e�[�g����J�����̃x�[�X���W���X�V
	StateMachine_Base::Update();

	//TestShakeUpdate();

	// �h��̑傫�����I�u�W�F�N�g���̍��W�ɔ��f
	ShakeUpdate();

	// �ŏI�I�ȃJ�����̍��W���X�V
	UpdateFinalPos();
}

void CameraMoveController::ShakeUpdate()
{
	if (!isShaking) return;

	// �h�炷�Ԋu���擾
	ShakeLevelParam& param = shakeLevelParameter[static_cast<u_int>(curShakeLevel)];

	/// �o�ߎ��Ԃ����Z
	shakeElapsedTime += GetCamera().DeltaTime();

	// �S�̎��Ԃ��߂�����I���
	if (shakeElapsedTime > shakeTime)
		OnEndShake();

	// �O��h�炵�Ă���Ԋu�����Ԃ��󂢂���
	else if (shakeElapsedTime - prevShakeElapsedTime > param.duration)
	{
		// �h��̃I�t�Z�b�g�l���X�V
		curShakeOffset = CalcShakeOffset(curShakeLevel, curShakeType);
		prevShakeElapsedTime = shakeElapsedTime;
	}
}

void CameraMoveController::UpdateFinalPos()
{
	Transform& camTransform = pCamera->GetTransform();

	// �J�����̌������l���������[���h��Ԃ̗h������߂�
	DXSimp::Vector3 worldShakeOffset;
	if (isShaking)
	{
		worldShakeOffset = camTransform.Right() * curShakeOffset.x +
			camTransform.Up() * curShakeOffset.y +
			camTransform.Forward() * curShakeOffset.z;
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
	pTargetTransform = _pTransform;
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

void CameraMoveController::BeginShake(ShakeLevel _level, ShakeType _type, float _time)
{
	// �O�h��Ă���̂�����Ȃ�㏑������
	// �p�����[�^�̃��Z�b�g
	curShakeLevel = _level;
	curShakeType = _type;
	shakeTime = _time;
	shakeElapsedTime = 0.0f;
	prevShakeElapsedTime = INIT_PREV_SHAKEELAPSETIME;

	// �h����J�n
	isShaking = true;
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

const Transform* CameraMoveController::GetFollowTransform() const
{
	return pTargetTransform;
}

bool CameraMoveController::GetHaveTarget() const
{
	return pTargetTransform != nullptr;
}

const DirectX::SimpleMath::Vector3& CameraMoveController::GetLookAtWorldPos() const
{
	if (!pLookAtTransform) return DirectX::SimpleMath::Vector3::Zero;

	return pLookAtTransform->GetWorldPosByTargetObj();
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

	// �h�ꃌ�x��
	auto& shakeDatas = data["shakeLevels"];
	for (auto& level : shakeLevelParameter)
	{
		nlohmann::json levelData;

		levelData["duration"] = level.duration;
		levelData["power"] = level.power;

		shakeDatas.push_back(levelData);
	}


	return data;
}

void CameraMoveController::Load(const nlohmann::json& _data)
{
	LoadStates(_data);

	// ���x�����Ƃ̗h��̃p�����[�^
	nlohmann::json shakeDatas;
	if (HashiTaku::LoadJsonDataArray("shakeLevels", shakeDatas, _data))
	{
		u_int levelId = 0;
		for (auto& shData : shakeDatas)
		{
			// �z��͈̔͊O�w�肷��Ȃ�
			if (levelId >= static_cast<u_int>(ShakeLevel::MaxNum)) break;

			auto& shLvParam = shakeLevelParameter[levelId];
			HashiTaku::LoadJsonFloat("duration", shLvParam.duration, shData);
			HashiTaku::LoadJsonFloat("power", shLvParam.power, shData);
			levelId++;
		}
	}
}

void CameraMoveController::OnEndShake()
{
	isShaking = false;
	curShakeOffset = DXSimp::Vector3::Zero;
}

std::string CameraMoveController::GetStateName(CameraMoveState_Base::CameraState _state)
{
	return 	std::string(
		magic_enum::enum_name<CameraMoveState_Base::CameraState>(_state)
	);
}

DirectX::SimpleMath::Vector3 CameraMoveController::CalcShakeOffset(ShakeLevel _level, ShakeType _type)
{
	DXSimp::Vector3 offset;

	// �h�炷�傫���擾
	u_int levelId = static_cast<u_int>(curShakeLevel);
	ShakeLevelParam& param = shakeLevelParameter[levelId];

	// �����ŗh��̃I�t�Z�b�g�l���擾
	switch (_type)
	{
	case ShakeType::Vertical:
		offset.y = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		break;

	case ShakeType::Horizon:
		offset.x = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		break;

	case ShakeType::Both:
		offset.x = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		offset.y = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		break;
	}

	return offset;
}

CameraMoveState_Base& CameraMoveController::CastCamState(HashiTaku::StateNode_Base& _stateBase)
{
	return static_cast<CameraMoveState_Base&>(_stateBase);
}

void CameraMoveController::TestShakeUpdate()
{
	const GameKey& key = MainApplication::GetInput().GetKeyboard();
	if (key.GetKeyDown(DIK_H))
		BeginShake(testLevel, testType, testTime);
	if (key.GetKeyDown(DIK_J))
	{
		u_int shakeId = static_cast<u_int>(testLevel);
		shakeId++;
		if (shakeId >= static_cast<u_int>(ShakeLevel::MaxNum))
		{
			shakeId = 0;
		}

		testLevel = static_cast<ShakeLevel>(shakeId);
	}
	if (key.GetKeyDown(DIK_K))
	{
		u_int shakeId = static_cast<u_int>(testType);
		shakeId++;
		if (shakeId >= static_cast<u_int>(ShakeType::MaxNum))
		{
			shakeId = 0;
		}

		testType = static_cast<ShakeType>(shakeId);
	}
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

	ImGuiShakeLevel();
}

void CameraMoveController::ImGuiShakeLevel()
{
#ifdef EDIT
	if (!ImGuiMethod::TreeNode("ShakeLevel")) return;

	std::string s = "testLevel:" + std::string(magic_enum::enum_name<ShakeLevel>(testLevel));
	ImGui::Text(s.c_str());
	std::string s1 = "testType:" + std::string(magic_enum::enum_name<ShakeType>(testType));
	ImGui::Text(s1.c_str());
	ImGui::DragFloat("testTime", &testTime);

	// �e�h��̋�����ݒ�
	u_int levelCnt = static_cast<u_int>(shakeLevelParameter.size());
	for (u_int l_i = 0; l_i < levelCnt; l_i++)
	{
		std::string levelName =
			std::string(magic_enum::enum_name<ShakeLevel>(static_cast<ShakeLevel>(l_i)));

		if (ImGuiMethod::TreeNode(levelName))
		{
			ImGui::DragFloat("duration", &shakeLevelParameter[l_i].duration, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("power", &shakeLevelParameter[l_i].power, 0.01f, 0.0f, 100.0f);

			ImGui::TreePop();
		}
	}

	ImGui::TreePop();
#endif // EDIT
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

