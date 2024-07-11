#include "pch.h"
#include "SF_GameObject.h"
#include "GameObject.h"

// �V�X�e��
#include "InSceneSystemManager.h"

using namespace SceneFunction;

// �֗��֐� �錾 
GameObject& AddSceneObject(std::unique_ptr<GameObject> _pAddObject);

GameObject& Object::Instantiate(const GameObject& _obj, const DirectX::SimpleMath::Vector3& _worldPos)
{
	// �V�����m�ہA�R�s�[����
	std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>(_obj);

	// �������W�ݒ�
	pCreateObject->transform.position = _worldPos;

	// �V�[���I�u�W�F�N�g�ɒǉ�
	GameObject& retuenObject = AddSceneObject(std::move(pCreateObject));

	return retuenObject;
}

GameObject& SceneFunction::Object::CreateEmpty(std::string _objectName)
{
	std::unique_ptr<GameObject> pCreateObject = std::make_unique<GameObject>();
	pCreateObject->SetName(_objectName);

	// �V�[���I�u�W�F�N�g�ɓo�^����
	return AddSceneObject(std::move(pCreateObject));
}

// �֗��֐� ��`

GameObject& AddSceneObject(std::unique_ptr<GameObject> _pAddObject)
{
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

	return *sceneObjects.SetObject(std::move(_pAddObject));
}
