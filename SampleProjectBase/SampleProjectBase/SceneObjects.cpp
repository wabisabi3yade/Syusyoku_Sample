#include "pch.h"
#include "SceneObjects.h"

SceneObjects::SceneObjects()
{
}

SceneObjects::~SceneObjects()
{
	list.clear();	// ‰ð•ú‚·‚é
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

void SceneObjects::SetObject(std::string _objectName, std::unique_ptr<GameObject> _objPtr)
{
	list.insert(std::pair<std::string, std::unique_ptr<GameObject>>(_objectName, std::move(_objPtr)));
}
