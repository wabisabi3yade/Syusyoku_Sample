#include "pch.h"
#include "CP_CameraMove.h"
#include "GameObject.h"

#include "CP_Camera.h"

#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;


CP_CameraMove::CP_CameraMove() :
	pTargetTransform(nullptr), pCamera(nullptr)
{
}

void CP_CameraMove::Init()
{
	// �R���g���[�����쐬
	pMoveController = std::make_unique<CameraMoveController>();
}

bool CP_CameraMove::IsCanUpdate()
{
#ifdef EDIT
	if (pTargetTransform == nullptr) return false;
#endif // EDIT

	return true;
}

void CP_CameraMove::Start()
{
	// �J�������擾����
	pCamera = gameObject->GetComponent<CP_Camera>();
	if (pCamera == nullptr)
		assert(!"Camera�R���|�[�l���g�����Ă�������");

	pMoveController->Begin(*pCamera);	// ��������
	pMoveController->SetTargetTransform(pTargetTransform);
}

void CP_CameraMove::LateUpdate()
{
	if (!IsCanUpdate()) return;

	// �J�����ړ����X�V
	pMoveController->UpdateCall();
}

void CP_CameraMove::SetTargetTransform(const Transform* _targetTransform)
{
	pTargetTransform = _targetTransform;
	pMoveController->SetTargetTransform(_targetTransform);
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
	std::string text = "target:";

	if (pTargetTransform)
		text += pTargetTransform->GetObjectName();

	ImGui::Text(text.c_str());
	static char str[IM_INPUT_BUF];
	ImGui::InputText("targetName", str, IM_INPUT_BUF);

	if (ImGui::Button("Set"))
	{
		SceneObjects& objects = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* go = objects.GetSceneObject(str);
		if (go)
			pTargetTransform = &go->GetConstTransform();
	}
}

nlohmann::json CP_CameraMove::Save()
{
	auto data = Component::Save();

	// �^�[�Q�b�g�̃I�u�W�F�N�g���Z�[�u
	if (pTargetTransform)
		data["target"] = pTargetTransform->GetObjectName();

	// �ړ��R���g���[���̃Z�[�u
	data["moveController"] = pMoveController->Save();

	return data;
}

void CP_CameraMove::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	// �^�[�Q�b�g������I�u�W�F�N�g�擾
	std::string targetObjName;
	LoadJsonString("target", targetObjName, _data);

	// �V�[���Ȃ�����T��
	SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
	GameObject* pTargetObject = sceneObjs.GetSceneObject(targetObjName);
	if (!pTargetObject) return;

	// �Ώۃg�����X�t�H�[���ɃZ�b�g
	pTargetTransform = &pTargetObject->GetConstTransform();

	// �R���g���[���̃��[�h
	nlohmann::json controllerData;
	if (HashiTaku::LoadJsonData("moveController", controllerData, _data))
	{
		pMoveController->Load(controllerData);
	}
}
