#include "pch.h"
#include "SceneObjects.h"
#include "SF_Define.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
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
		auto objItr = objList.begin();
		for (; objItr != objList.end();)
		{
			GameObject& go = *(*objItr).second;
			go.LateUpdateCall();

			++objItr;

			// ���X�g����폜����̂ł����ōs��
			if (go.isDestroy)
				go.Destroy();
		}

		auto uiItr = uiList.begin();
		for (; uiItr != uiList.end();)
		{
			GameObject& go = *(*uiItr).second;
			go.LateUpdateCall();

			++uiItr;

			// ���X�g����폜����̂ł����ōs��
			if (go.isDestroy)
				go.Destroy();
		}
	}

	void SceneObjects::ObjectDraw()
	{
		// 3D��ԏ�̃I�u�W�F�N�g�`��
		for (auto& obj : objList)
		{
			obj.second->DrawCall();
		}
	}

	void SceneObjects::UIDraw()
	{
		UIDrawSetup();

		// 2D��ԁiUI�j�̃I�u�W�F�N�g
		for (auto& ui : drawUiList)
		{
			ui->DrawCall();
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

		// �`��UI���X�g�ɒǉ�
		SetDrawUIList(*takeoutObj);

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

		RemoveDrawUIList(*takeoutObj);

		// UI���X�g�ɒǉ�����
		objList[objectName] = std::move(takeoutObj);
	}

	void SceneObjects::SetDrawUIList(GameObject& _uiObj)
	{
		// UI�I�u�W�F�N�g���H
		if (_uiObj.GetLayer() != Layer::Type::UI) return;

		// ���X�g���ɂ���Ȃ�
		auto itr = std::find(drawUiList.begin(), drawUiList.end(), &_uiObj);
		if (itr != drawUiList.end()) return;

		drawUiList.push_back(&_uiObj);	// �ǉ�

		SortUiList();	// z�l���ɕ��ׂ�
	}

	void SceneObjects::RemoveDrawUIList(GameObject& _uiObj)
	{
		drawUiList.remove(&_uiObj);
	}

	std::string SceneObjects::PrefabFileParh()
	{
		return "assets/data/prefab/";
	}

	void SceneObjects::CreatePrefab(const std::string& _prefabName)
	{
		// �v���n�u�̃p�X��
		std::string prefabPath = PrefabFileParh();
		prefabPath += _prefabName + ".json";

		// �t�@�C������json��ǂݍ���
		std::ifstream f(prefabPath);
		if (!f.is_open())
		{
			HASHI_DEBUG_LOG(prefabPath + " �v���n�u�t�@�C�����J���܂���ł���");
			return;
		}

		const json prefabData = json::parse(f);
		// �v���n�u�����[�h
		LoadObject(prefabData);
		LateLoadObject(prefabData);
		HASHI_DEBUG_LOG(_prefabName + " �쐬����");
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

		// �v���n�u����
		if (ImGui::Button("prefab") && createObjName[0] != '\0')
			CreatePrefab(createObjName);

		ImGui::End();

		// ���X�g�Ԉړ�������
		MoveList();
	}

	json SceneObjects::SaveObject(GameObject& _go)
	{
		json objData;
		objData["gameObject"] = _go.Save();

		Transform& transform = _go.GetTransform();

		// �q�I�u�W�F�N�g���Z�[�u
		auto& childData = objData["childs"];
		u_int childCnt = transform.GetChildCnt();
		for (u_int c_i = 0; c_i < childCnt; c_i++)
		{
			// �q�I�u�W�F�N�g���Z�[�u����
			GameObject& childGameObj = transform.GetChild(c_i)->GetGameObject();
			childData.push_back(SaveObject(childGameObj));
		}

		return objData;
	}

	void SceneObjects::ObjectToPrefab(GameObject& _toPrefabObject)
	{
		// �V�[���f�[�^
		json prefabData;

		// �V�[�����I�u�W�F�N�g���Z�[�u
		prefabData = SaveObject(_toPrefabObject);

		// �t�@�C������ݒ�
		std::string savePathStr;
		savePathStr = PrefabFileParh();	// �p�X
		savePathStr += _toPrefabObject.GetName();	// �t�@�C����
		savePathStr += ".json";	// �g���qPrefabFileParh()

		std::ofstream f(savePathStr);
		auto str = prefabData.dump(4);
		auto len = str.length();
		f.write(str.c_str(), len);

		HASHI_DEBUG_LOG(_toPrefabObject.GetName() + "�̃v���n�u���ɐ������܂���");
	}

	bool SceneObjects::LoadObject(const json& _gameObjectData)
	{
		// �I�u�W�F�N�g��ǂݍ���
		json objData;
		if (!LoadJsonData("gameObject", objData, _gameObjectData))
		{
			HASHI_DEBUG_LOG("�I�u�W�F�N�g���ǂݍ��߂܂���ł���");
			return false;
		}

		// ���O����Q�[���I�u�W�F�N�g���쐬
		std::string objectName;
		LoadJsonString("name", objectName, objData);
		GameObject& go = SceneFunction::ObjectFunc::CreateEmpty(objectName);
		go.Load(objData);

		// UI�Ȃ�UI���X�g
		if (IsUI(go))
			MoveToUIList(go);


		// �q�I�u�W�F�N�g��ǂݍ���
		json childDatas;
		if (!LoadJsonDataArray("childs", childDatas, _gameObjectData))
		{
			return false;
		}

		// �ċA�I�ɓǂݍ���
		for (auto& childData : childDatas)
			LoadObject(childData);

		return true;
	}

	bool SceneObjects::LateLoadObject(const json& _gameObjectData)
	{
		// �I�u�W�F�N�g��ǂݍ���
		json objData;
		if (!LoadJsonData("gameObject", objData, _gameObjectData))
		{
			HASHI_DEBUG_LOG("�I�u�W�F�N�g���ǂݍ��߂܂���ł���");
			return false;
		}

		std::string objectName;
		LoadJsonString("name", objectName, objData);
		GameObject* go = GetSceneObject(objectName);

		go->LateLode(objData);

		// �q�I�u�W�F�N�g��ǂݍ���
		json childDatas;
		if (!LoadJsonDataArray("childs", childDatas, _gameObjectData))
		{
			return false;
		}

		// �ċA�I�ɓǂݍ���
		for (auto& childData : childDatas)
			LateLoadObject(childData);

		return true;
	}

	bool SceneObjects::ImGuiSettingObject(GameObject& _gameObject)
	{
#ifdef  EDIT

		bool isDelete = false;

		const std::string& objName = _gameObject.GetName();
		if (ImGuiMethod::TreeNode(objName))
		{
			// �폜
			isDelete = ImGui::Button("Delete");

			ImGui::SameLine();

			// �v���n�u���쐬
			if (ImGui::Button("Prefab"))
				ObjectToPrefab(_gameObject);

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

	bool SceneObjects::SortUIPosZFunction(const GameObject* _a1, const GameObject* _a2)
	{
		return _a1->GetConstTransform().GetPosition().z > _a2->GetConstTransform().GetPosition().z;
	}

	GameObject* SceneObjects::SetObject(std::unique_ptr<GameObject> _objPtr)
	{
		// ���O���󂩊m�F
		CheckEmptyName(*_objPtr.get());

		// ���O���d�����Ă��邩�m�F
		CheckDuplicationName(*_objPtr.get(), objList);
		CheckDuplicationName(*_objPtr.get(), uiList);

		// �Z�b�g���郊�X�g�i�I�u�W�F�N�g����UI���j
		SceneObjectList* setList = &objList;

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
			RemoveDrawUIList(*(*itr).second);
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

	void SceneObjects::SortUiList()
	{
		// �\�[�g����
		drawUiList.sort(SortUIPosZFunction);
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

	json SceneObjects::SaveObjectList()
	{
		json objectData;

		for (auto& obj : objList)
		{
			Transform& transform = obj.second->GetTransform();

			// �e�I�u�W�F�N�g������Ȃ�
			if (transform.GetParent()) continue;
			objectData.push_back(SaveObject(*obj.second));	// �Z�[�u
		}


		for (auto& ui : uiList)
		{
			Transform& transform = ui.second->GetTransform();

			// �e�I�u�W�F�N�g������Ȃ�
			if (transform.GetParent()) continue;
			objectData.push_back(SaveObject(*ui.second));	// �Z�[�u
		}

		return objectData;
	}

	void SceneObjects::LoadObjectList(const json& _objectsData)
	{
		// �I�u�W�F�N�g���쐬����
		for (const auto& data : _objectsData)
		{
			// �I�u�W�F�N�g�����[�h
			LoadObject(data);
		}

		// �I�u�W�F�N�g�����[�h
		for (auto& data : _objectsData)
		{
			LateLoadObject(data);
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
			if (pObj->GetLayer() == Layer::Type::UI)
				MoveToUIList(*pObj);
			else
				MoveToObjList(*pObj);
		}

		tmpMoveList.clear();
	}
}