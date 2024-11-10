#include "pch.h"
#include "SceneObjects.h"
#include "SF_Define.h"
#include "InSceneSystemManager.h"

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
	for (auto& obj : objList)
	{
		obj.second->AwakeCall();
	}

	for (auto& ui : uiList)
	{
		ui.second->AwakeCall();
	}
}

void SceneObjects::Start()
{
	for (auto& obj : objList)
	{
		obj.second->StartCall();
	}

	for (auto& ui : uiList)
	{
		ui.second->StartCall();
	}
}

void SceneObjects::Update()
{
	for (auto& obj : objList)
	{
		obj.second->UpdateCall();
	}

	for (auto& ui : uiList)
	{
		ui.second->UpdateCall();
	}
}

void SceneObjects::LateUpdate()
{
	for (auto& obj : objList)
	{
		obj.second->LateUpdateCall();
	}

	for (auto& ui : uiList)
	{
		ui.second->LateUpdateCall();
	}
}

void SceneObjects::Draw()
{
	// 3D��ԏ�̃I�u�W�F�N�g�`��
	for (auto& obj : objList)
	{
		obj.second->DrawCall();
	}

	UIDrawSetup();
	
	// 2D��ԁiUI�j�̃I�u�W�F�N�g
	for (auto& ui : uiList)
	{
		ui.second->DrawCall();
	}

	// �������e�ɖ߂�
	UIDrawEnd();
}

void SceneObjects::MoveToUIList(GameObject& _gameObject)
{
	// UI����Ȃ��Ȃ�
	if (!IsUI(_gameObject)) return;

	// �I�u�W�F�N�g������o��
	std::string objectName;
	std::unique_ptr<GameObject> takeoutObj;
	if (!TakeOutObject(_gameObject, objectName, takeoutObj, objList)) return;

	// UI���X�g�ɒǉ�����
	uiList[objectName] = std::move(takeoutObj);
}

void SceneObjects::MoveToObjList(GameObject& _gameObject)
{
	// UI�Ȃ�
	if (IsUI(_gameObject)) return;

	// �I�u�W�F�N�g������o��
	std::string objectName;
	std::unique_ptr<GameObject> takeoutObj;
	if (!TakeOutObject(_gameObject, objectName, takeoutObj, uiList)) return;

	// UI���X�g�ɒǉ�����
	objList[objectName] = std::move(takeoutObj);
}

void SceneObjects::ImGuiDebug()
{
	ImGui::Begin("SceneObjects");

	// �I�u�W�F�N�g
	for (auto& obj : objList)
	{
		// �e�I�u�W�F�N�g������Ȃ�
		if (obj.second->GetTransform().GetParent()) continue;

		bool isDelete = ImGuiSettingObject(*obj.second);

		if (isDelete) break;
	}

	// �I�u�W�F�N�g
	for (auto& ui : uiList)
	{
		// �e�I�u�W�F�N�g���Ȃ��Ȃ�
		if (ui.second->GetTransform().GetParent()) continue;

		bool isDelete = ImGuiSettingObject(*ui.second);

		if (isDelete) break;
	}

	ImGui::Dummy(ImVec2(0, 10));

	// ����
	static char createObjName[IM_INPUT_BUF];
	ImGui::InputText("createName", createObjName, IM_INPUT_BUF);
	if (ImGui::Button("new Object") && createObjName[0] != '\0')
		SceneFunction::ObjectFunc::CreateEmpty(createObjName);

	ImGui::End();

	// ���X�g�Ԉړ�������
	MoveList();
}

bool SceneObjects::ImGuiSettingObject(GameObject& _gameObject)
{
#ifdef  EDIT

	bool isDelete = false;

	const std::string& objName = _gameObject.GetName();
	if (ImGuiMethod::TreeNode(objName))
	{
		isDelete = ImGui::Button("Delete");
		_gameObject.ImGuiCall();
		ImGui::TreePop();
	}

	if (isDelete)
	{
		_gameObject.Destroy();
		return true;
	}

	// �q�I�u�W�F�N�g��\��
	Transform& t = _gameObject.GetTransform();
	ImGui::Indent();
	for (u_int c_i = 0; c_i < t.GetChildCnt(); c_i++)
	{
		isDelete = ImGuiSettingObject(t.GetChild(c_i)->GetGameObject());

		if (isDelete)
			return true;
	}
	ImGui::Unindent();

#endif //  EDIT
	return false;
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

void SceneObjects::DeleteGameObject(GameObject& _deleteObj)
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

void SceneObjects::MoveTmpList(GameObject& _targetObj)
{
	// �ړ��z��ɒǉ�����
	tmpMoveList.push_back(&_targetObj);
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

		if (IsUI(go))
			MoveToUIList(go);
			
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

void SceneObjects::CheckDuplicationName(GameObject& _gameObject, SceneObjectList& _objects)
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

bool SceneObjects::TakeOutObject(const GameObject& _targetObject, std::string& _objectName, std::unique_ptr<GameObject>& _outObject, SceneObjectList& _objectList)
{
	// �����A�h���X��T�����Ă���
	for (auto itr = _objectList.begin(); itr != _objectList.end(); itr++)
	{
		if (&_targetObject == itr->second.get())
		{
			// �A�h���X�Ɩ��O��n��
			_objectName = itr->first;
			_outObject = std::move(itr->second);

			// �폜����
			_objectList.erase(itr);

			return true;
		}
	}

	return false;
}

bool SceneObjects::IsUI(GameObject& _gameObject)
{
	using namespace HashiTaku;

	Layer::Type layer = _gameObject.GetLayer();
	return layer == Layer::Type::UI ? true : false;
}

void SceneObjects::UIDrawSetup()
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();

	// UI�`��̂��ߕ��s���e�ɐ؂�ւ�
	camera.SetOrthographic();

	// �[�x�o�b�t�@�����Z�b�g����
	renderer.GetDeviceContext()->ClearDepthStencilView(
		renderer.GetDepthStencil(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		1.0f, 
		0
	);
}

void SceneObjects::UIDrawEnd()
{
	CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();
	camera.SetPerspective();
}

void SceneObjects::MoveList()
{
	if (tmpMoveList.empty()) return;

	// �ꎞ�ړ��z��ɓ����Ă���I�u�W�F�N�g���ړ�������
	for (auto& pObj : tmpMoveList)
	{
		if (pObj->GetLayer() == HashiTaku::Layer::Type::UI)
			MoveToUIList(*pObj);
		else
			MoveToObjList(*pObj);
	}

	tmpMoveList.clear();
}

