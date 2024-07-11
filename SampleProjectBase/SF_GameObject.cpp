#include "pch.h"
#include "SF_GameObject.h"
#include "GameObject.h"

// システム
#include "InSceneSystemManager.h"

using namespace SceneFunction;

// 便利関数 宣言 
GameObject& AddSceneObject(std::unique_ptr<GameObject> _pAddObject);

GameObject& Object::Instantiate(const GameObject& _obj, const DirectX::SimpleMath::Vector3& _worldPos)
{
	// 新しく確保、コピーする
	std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>(_obj);

	// 初期座標設定
	pCreateObject->transform.position = _worldPos;

	// シーンオブジェクトに追加
	GameObject& retuenObject = AddSceneObject(std::move(pCreateObject));

	return retuenObject;
}

GameObject& SceneFunction::Object::CreateEmpty(std::string _objectName)
{
	std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>();
	pCreateObject->SetName(_objectName);

	// シーンオブジェクトに登録する
	return AddSceneObject(std::move(pCreateObject));
}

// 便利関数 定義

GameObject& AddSceneObject(std::unique_ptr<GameObject> _pAddObject)
{
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

	return *sceneObjects.SetObject(std::move(_pAddObject));
}
