#pragma once

class SceneObjects;
class GameObject;

// SceneObjects�ɓo�^����N���X
class SObjectRegister : public Singleton_Base<SObjectRegister>
{
	friend class Singleton_Base<SObjectRegister>;

	SceneObjects* pSceneObjests;
	SObjectRegister() : pSceneObjests(nullptr) {};
	~SObjectRegister(){};
public:
	void AddSceneObject(std::unique_ptr<GameObject> _gameObject);	// �I�u�W�F�N�g��ǉ�����
	void PopObject(GameObject& _deleteObj);	// �I�u�W�F�N�g���폜����

	// �V�[���I�u�W�F�N�g�N���X��o�^����
	void SetSceneObjects(SceneObjects& _sceneObjects) { pSceneObjests = &_sceneObjects; }
};

