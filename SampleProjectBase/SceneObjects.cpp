#include "pch.h"
#include "SceneObjects.h"
#include <string>
#include "Camera.h"

typedef std::unordered_map<std::string, std::unique_ptr<GameObject>> ObjectList;

SceneObjects::SceneObjects()
{
}

SceneObjects::~SceneObjects()
{
	objList.clear();	// �������
	uiList.clear();
}

void SceneObjects::Update()
{
#ifdef EDIT
	ImGui::Begin(ShiftJisToUtf8("�V�[���I�u�W�F�N�g").c_str());
#endif // EDIT

	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->UpdateBase();
		itr->second->ImGuiSet();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->UpdateBase();
		itr->second->ImGuiSet();
	}

#ifdef EDIT
	ImGui::End();
#endif // EDIT


}

void SceneObjects::LateUpdate()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->LateUpdateBase();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->LateUpdateBase();
	}
}

void SceneObjects::Draw()
{
	// 3D��ԏ�̃I�u�W�F�N�g�`��
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->DrawBase();
	}


	// �����s���e�������� //


	//------------------//

	// 2D��ԁiUI�j�̃I�u�W�F�N�g
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->DrawBase();
	}
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

