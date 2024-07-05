#include "pch.h"
#include "SF_GameObject.h"
#include "GameObject.h"

// システム
#include "InSceneSystemManager.h"

GameObject& SceneFunction::CreateObject(const GameObject& _obj, DirectX::SimpleMath::Vector3 _worldPos)
{
	// 新しく確保、コピーする
	std::unique_ptr<GameObject> create = std::make_unique<GameObject>();
	GameObject* retPtr = create.get();
	*retPtr = _obj;

	// 初期座標設定
	create->transform.position = _worldPos;

	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();
	// シーンオブジェクト配列に追加
	sceneObjects.SetObject(retPtr->GetName(), std::move(create));

	return *retPtr;
}
