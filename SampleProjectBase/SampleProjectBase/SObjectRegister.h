#pragma once

class SceneObjects;
class GameObject;

// SceneObjectsに登録するクラス
class SObjectRegister : public Singleton_Base<SObjectRegister>
{
	friend class Singleton_Base<SObjectRegister>;

	SceneObjects* pSceneObjests;
	SObjectRegister() : pSceneObjests(nullptr) {};
	~SObjectRegister(){};
public:
	void AddSceneObject(std::unique_ptr<GameObject> _gameObject);	// オブジェクトを追加する
	void PopObject(GameObject& _deleteObj);	// オブジェクトを削除する

	// シーンオブジェクトクラスを登録する
	void SetSceneObjects(SceneObjects& _sceneObjects) { pSceneObjests = &_sceneObjects; }
};

