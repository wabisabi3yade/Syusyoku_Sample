#include "pch.h"
#include "SObjectRegister.h"
#include "SceneObjects.h"
#include "GameObject.h"

void SObjectRegister::AddSceneObject(std::unique_ptr<GameObject> _gameObject)
{
	std::string name = _gameObject->GetName();
	pSceneObjests->SetObject(name, std::move(_gameObject));
}

void SObjectRegister::PopObject(GameObject& _deleteObj)
{
	pSceneObjests->DeleteObj(_deleteObj);
}
