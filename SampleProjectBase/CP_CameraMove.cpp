#include "pch.h"
#include "CP_CameraMove.h"
#include "GameObject.h"

#include "CP_Camera.h"

#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;


CP_CameraMove::CP_CameraMove() :
	pFollowTransform(nullptr), pCamera(nullptr), pTargetObject(nullptr)
{
}

CP_CameraMove::~CP_CameraMove()
{
	if (pTargetObject)
		pTargetObject->RemoveTargeter(*this);

	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->RemoveCamMove(*this);
	}
}

void CP_CameraMove::Init()
{
	// �R���g���[�����쐬
	pMoveController = std::make_unique<CameraMoveController>();
}

bool CP_CameraMove::IsCanUpdate()
{
#ifdef EDIT
	if (pFollowTransform == nullptr) return false;
#endif // EDIT

	return true;
}

void CP_CameraMove::FindFollowObject()
{
	// �V�[��������T��
	SceneObjects& objects =
		InSceneSystemManager::GetInstance()->GetSceneObjects();

	GameObject* go = objects.GetSceneObject(followObjName);

	if (go)	// ��������g�����X�t�H�[�����擾
		pFollowTransform = &go->GetConstTransform();
}

void CP_CameraMove::Awake()
{
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->SetCameraMove(*this);
	}
}

void CP_CameraMove::Start()
{
	// �Ǐ]��̃I�u�W�F�N�g��T��
	FindFollowObject();

	// �J�������擾����
	pCamera = gameObject->GetComponent<CP_Camera>();
	if (pCamera == nullptr)
		assert(!"Camera�R���|�[�l���g�����Ă�������");

	pMoveController->SetFollowTransform(pFollowTransform);
	pMoveController->Begin(*pCamera);	// ��������
}

void CP_CameraMove::LateUpdate()
{
	if (!IsCanUpdate()) return;

	// �J�����ړ����X�V
	pMoveController->UpdateCall();
}

void CP_CameraMove::SetFollowTransform(const Transform* _followTransform)
{
	pFollowTransform = _followTransform;
	pMoveController->SetFollowTransform(_followTransform);
}

void CP_CameraMove::SetTargetObject(ITargetAccepter& _targetObject)
{
	// �^�[�Q�b�g�I�u�W�F�N�g���擾
	pTargetObject = &_targetObject;
	pTargetObject->SetTargeter(*this);
	pMoveController->SetLookAtObject(&_targetObject);
}

void CP_CameraMove::RemoveNotify(const ITargetAccepter& _removeObj)
{
	// �^�[�Q�b�g������������Ă����Ȃ�
	if (pTargetObject != &_removeObj) return;

	pTargetObject = nullptr;
	pMoveController->SetLookAtObject(nullptr);
}

void CP_CameraMove::OnPlayerWin(const Transform& _targetTransform)
{
	pMoveController->OnPlayerWin(_targetTransform);
}

void CP_CameraMove::ImGuiDebug()
{
	ImGuiSetTarget();

	// �R���g���[���̕ҏW
	pMoveController->ImGuiCall();
}

void CP_CameraMove::ImGuiSetTarget()
{
	// �^�[�Q�b�g�̖��O��T���ăZ�b�g����
	std::string text = "follow:" + followObjName;
	ImGui::Text(text.c_str());

	// ���͂ŕύX
	static char str[IM_INPUT_BUF];
	ImGui::InputText("objName", str, IM_INPUT_BUF);
	if (ImGui::Button("Set Follow"))
	{
		followObjName = str;
	}
}

nlohmann::json CP_CameraMove::Save()
{
	auto data = Component::Save();

	// �^�[�Q�b�g�̃I�u�W�F�N�g���Z�[�u
	data["target"] = followObjName;

	// �ړ��R���g���[���̃Z�[�u
	data["moveController"] = pMoveController->Save();

	return data;
}

void CP_CameraMove::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	// �^�[�Q�b�g������I�u�W�F�N�g�擾
	LoadJsonString("target", followObjName, _data);

	// �R���g���[���̃��[�h
	nlohmann::json controllerData;
	if (HashiTaku::LoadJsonData("moveController", controllerData, _data))
	{
		pMoveController->Load(controllerData);
	}
}
