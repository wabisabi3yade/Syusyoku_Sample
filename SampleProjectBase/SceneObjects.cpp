#include "pch.h"
#include "SceneObjects.h"
#include <string>

#include "SF_Define.h"

typedef std::unordered_map<std::string, std::unique_ptr<GameObject>> ObjectList;

SceneObjects::SceneObjects()
{
}

SceneObjects::~SceneObjects()
{
	objList.clear();	// �������
	uiList.clear();
}

void SceneObjects::Awake()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->AwakeCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->AwakeCall();
	}
}

void SceneObjects::Start()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->StartCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->StartCall();
	}
}

void SceneObjects::Update()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->UpdateCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->UpdateCall();
	}
}

void SceneObjects::LateUpdate()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->LateUpdateCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->LateUpdateCall();
	}
}

void SceneObjects::UpdateRigidBody()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->AwakeCall();
	}
}

void SceneObjects::Draw()
{
	// 3D��ԏ�̃I�u�W�F�N�g�`��
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->DrawCall();
	}


	// �����s���e�������� //


	//------------------//

	// 2D��ԁiUI�j�̃I�u�W�F�N�g
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->DrawCall();
	}
}

void SceneObjects::ImGuiSetting()
{
#ifdef EDIT

	ImGui::Begin(ShiftJisToUtf8("�V�[���I�u�W�F�N�g").c_str());

	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->ImGuiCall();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->ImGuiCall();
	}

	ImGui::Dummy(ImVec2(0, 10));
	constexpr u_int buf = 256;

	// ����
	static char createObjName[buf];
	ImGui::InputText("createName", createObjName, buf);
	if (ImGui::Button("new Object") && createObjName[0] != '\0')
		SceneFunction::ObjectFunc::CreateEmpty(createObjName);

	// �폜
	static char deleteObjName[buf];
	ImGui::InputText("deleteName", deleteObjName, buf);
	if (ImGui::Button("delete"))
		SceneFunction::ObjectFunc::DeleteObject(deleteObjName);

	ImGui::End();
#endif // EDIT
}

GameObject* SceneObjects::SetObject(std::unique_ptr<GameObject> _objPtr)
{
	// ���O���󂩊m�F
	CheckEmptyName(*_objPtr.get());

	// ���O���d�����Ă��邩�m�F
	CheckDuplicationName(*_objPtr.get(), objList);
	CheckDuplicationName(*_objPtr.get(), uiList);

	// �Z�b�g���郊�X�g�i�I�u�W�F�N�g����UI���j
	ObjectList* setList = &objList;

	// UI�Ȃ�
	if (IsUI(*_objPtr.get()))
		setList = &uiList;	// UI���X�g�ɃZ�b�g����

	GameObject* retPtr = _objPtr.get();

	// �z��ɓ����
	std::string setName = _objPtr->GetName();
	setList->insert(std::pair<std::string, std::unique_ptr<GameObject>>(setName, std::move(_objPtr)));

	return retPtr;
}

void SceneObjects::DeleteObj(GameObject& _deleteObj)
{
	// �z����ɓ����A�h���X��T��
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		if (itr->second.get() != &_deleteObj) continue;

		// ��������
		objList.erase(itr);	// �폜����
		return;
	}

	// �z����ɓ����A�h���X��T��
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		if (itr->second.get() != &_deleteObj) continue;

		// ��������
		uiList.erase(itr);	// �폜����
		return;
	}

	HASHI_DEBUG_LOG(_deleteObj.GetName() + "�̓V�[�����ɂ���܂���");
}

GameObject* SceneObjects::GetSceneObject(const std::string& _objectName)
{
	// ���3D��ԏ�̃I�u�W�F�N�g����T���@���@����������UI�̕���T��

	// ���O����T��
	auto itr = objList.find(_objectName);
	if (itr == objList.end())
	{
		// �I�u�W�F�N�g�ɖ���������Ui�ŒT��
		itr = uiList.find(_objectName);

		// ����ł��Ȃ�������
		if (itr == uiList.end())
		{
			std::string message = _objectName + "���X�g���ɖ��O�̃I�u�W�F�N�g������܂���ł���";
			HASHI_DEBUG_LOG(message);
			return nullptr;
		}
	}

	return itr->second.get();
}

nlohmann::json SceneObjects::SaveObject()
{
	nlohmann::json objectData;

	for (auto& obj : objList)
	{
		objectData.push_back(obj.second->Save());
	}


	for (auto& ui : uiList)
	{
		objectData.push_back(ui.second->Save());
	}

	return objectData;
}

void SceneObjects::LoadObject(const nlohmann::json& _objectsData)
{
	// �I�u�W�F�N�g���쐬����
	for (const auto& data : _objectsData)
	{
		std::string objectName;
		HashiTaku::LoadJsonString("name", objectName, data);
		GameObject& go = SceneFunction::ObjectFunc::CreateEmpty(objectName);
		go.Load(data);
	}

	// �I�u�W�F�N�g�����[�h
	for (auto& data : _objectsData)
	{
		std::string objectName;
		HashiTaku::LoadJsonString("name", objectName, data);
		GameObject* go = GetSceneObject(objectName);

		go->LateLode(data);
	}
}

void SceneObjects::CheckEmptyName(GameObject& _gameObject)
{
	if (_gameObject.GetName() != "") return;

	_gameObject.SetName("Empty");
}

void SceneObjects::CheckDuplicationName(GameObject& _gameObject, Objects& _objects)
{
	std::string objName = _gameObject.GetName();

	std::string setName;

	// �������O�̃I�u�W�F�N�g���������ꍇ���O�̌��ɐ���������
	u_int loop = 0;
	while (true)	// �Z�b�g�ł���܂�
	{
		// �I�u�W�F�N�g�̖��O�{����
		std::string number = std::to_string(loop);
		if (loop == 0)
		{
			number = "";
		}
		setName = objName + number;

		auto itr = _objects.find(setName);
		// �T���Ė���������
		if (itr == _objects.end())
		{
			break;	// ���[�v���I���
		}

		// ����Ȃ�
		loop++;	// ���[�v�񐔂𑝂₷
	}

	_gameObject.SetName(setName);
}

bool SceneObjects::IsUI(GameObject& _gameObject)
{
	Layer::Type layer = _gameObject.GetLayer().GetType();
	return layer == Layer::Type::UI ? true : false;
}

