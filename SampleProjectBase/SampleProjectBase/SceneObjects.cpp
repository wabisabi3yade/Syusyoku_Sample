#include "pch.h"
#include "SceneObjects.h"
#include <string>

SceneObjects::SceneObjects()
{
}

SceneObjects::~SceneObjects()
{
	list.clear();	// �������
}

void SceneObjects::Update()
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		itr->second->Update();
	}
}

void SceneObjects::LateUpdate()
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		itr->second->LateUpdate();
	}
}

void SceneObjects::Draw()
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		itr->second->Draw();
	}
}

void SceneObjects::SetObject(const std::string& _objectName, std::unique_ptr<GameObject> _objPtr)
{
	// ���ɃI�u�W�F�N�g�̖��O������Ȃ�
	u_int loop = 0;
	std::string setName;	// �Z�b�g����Ƃ��̖��O
	while (true)	// �Z�b�g�ł���܂�
	{
		std::string number = std::to_string(loop);
		if (loop == 0)	// �ŏ��͉������Ȃ��悤�ɂ���
		{
			number = "";
		}
		
		setName = _objectName + number;	// �I�u�W�F�N�g�̖��O�{����

		auto itr = list.find(setName);
		if (itr == list.end())	// �T���ĂȂ�������
		{
			break;	// ���[�v���I���
		}

		// ����Ȃ�
		loop++;	// ���[�v�񐔂𑝂₷
	}

	list.insert(std::pair<std::string, std::unique_ptr<GameObject>>(setName, std::move(_objPtr)));
}
