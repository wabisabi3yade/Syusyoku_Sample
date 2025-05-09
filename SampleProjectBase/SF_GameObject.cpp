#include "pch.h"
#include "SF_GameObject.h"

// システム
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
	//	// 新しく確保、コピーする
	//	std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>(_obj);
	//
	//	// 初期座標設定
	//	pCreateObject->GetTransform().SetPosition(_worldPos);
	//
	//	// シーンオブジェクトに追加
	//	GameObject& retuenObject = AddSceneObject(std::move(pCreateObject));
	//
	//	return retuenObject;
	//}

	GameObject& SceneFunction::ObjectFunc::CreateEmpty(const std::string& _objectName)
	{
		std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>();
		pCreateObject->SetName(_objectName);

		// シーンオブジェクトに登録する
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