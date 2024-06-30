#include "pch.h"
#include "SceneObjects.h"
#include <string>
#include "Camera.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

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
#ifdef _DEBUG
	ImGui::Begin("SceneObjects");
#endif // _DEBUG

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

#ifdef _DEBUG
	ImGui::End();
#endif // _DEBUG


}

void SceneObjects::LateUpdate()
{
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->LateUpdate();
	}

	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->LateUpdate();
	}
}

void SceneObjects::Draw()
{
	// 3D��ԏ�̃I�u�W�F�N�g�`��
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		itr->second->Draw();
	}


	// �����s���e�������� //
	

	//------------------//
	
	// 2D��ԁiUI�j�̃I�u�W�F�N�g
	for (auto itr = uiList.begin(); itr != uiList.end(); itr++)
	{
		itr->second->Draw();
	}
}

GameObject* SceneObjects::SetObject(const std::string& _objectName, std::unique_ptr<GameObject> _objPtr)
{
	// ���ɃI�u�W�F�N�g�̖��O������Ȃ�
	u_int loop = 0;
	std::string setName;	// �Z�b�g����Ƃ��̖��O

	// �Z�b�g���郊�X�g�i�I�u�W�F�N�g����UI���j
	ObjectList* setList = &objList;

	if (_objPtr->GetLayer().GetType() == Layer::Type::UI)	// UI�Ȃ�
	{
		setList = &uiList;	// UI���X�g�ɃZ�b�g����
	}

	// �������O�̃I�u�W�F�N�g���������ꍇ���O�̌��ɐ���������
	while (true)	// �Z�b�g�ł���܂�
	{
		std::string number = std::to_string(loop);
		if (loop == 0)	// �P��ڂ͐��������Ȃ��悤�ɂ���
		{
			number = "";
		}

		setName = _objectName + number;	// �I�u�W�F�N�g�̖��O�{����

		auto itr = setList->find(setName);
		if (itr == setList->end())	// �T���Ė���������
		{
			break;	// ���[�v���I���
		}

		// ����Ȃ�
		loop++;	// ���[�v�񐔂𑝂₷
	}

	_objPtr->SetName(setName);	// �I�u�W�F�N�g�ɖ��O��ݒ�
	GameObject* retPtr = _objPtr.get();
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

void SceneObjects::Start()
{
	// Start����������
	for(auto itr = startComponents.begin(); itr != startComponents.end();)
	{
		(*itr)->Start();
		startComponents.erase(itr);
	}
}
