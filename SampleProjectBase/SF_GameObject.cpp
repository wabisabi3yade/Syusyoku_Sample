#include "pch.h"
#include "SF_GameObject.h"

// �V�X�e��
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	using namespace SceneFunction;

	GameObject& SceneFunction::ObjectFunc::AddSceneObject(std::unique_ptr<GameObject> _pAddObject)
	{
		SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

		return *sceneObjects.SetObject(std::move(_pAddObject));
	}

	//GameObject& ObjectFunc::Instantiate(const GameObject& _obj, const DXSimp::Vector3& _worldPos)
	//{
	//	// �V�����m�ہA�R�s�[����
	//	std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>(_obj);
	//
	//	// �������W�ݒ�
	//	pCreateObject->GetTransform().SetPosition(_worldPos);
	//
	//	// �V�[���I�u�W�F�N�g�ɒǉ�
	//	GameObject& retuenObject = AddSceneObject(std::move(pCreateObject));
	//
	//	return retuenObject;
	//}

	GameObject& SceneFunction::ObjectFunc::CreateEmpty(const std::string& _objectName)
	{
		std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>();
		pCreateObject->SetName(_objectName);

		// �V�[���I�u�W�F�N�g�ɓo�^����
		return AddSceneObject(std::move(pCreateObject));
	}

	void SceneFunction::ObjectFunc::DeleteObject(const std::string& _deleteName)
	{
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();

		GameObject* go = sceneObjs.GetSceneObject(_deleteName);
		if (!go) return;

		go->SetDestroy();
	}
}