#include "pch.h"
#include "SF_GameObject.h"
#include "GameObject.h"

// �V�X�e��
#include "InSceneSystemManager.h"

GameObject& SceneFunction::CreateObject(const GameObject& _obj, DirectX::SimpleMath::Vector3 _worldPos)
{
	// �V�����m�ہA�R�s�[����
	std::unique_ptr<GameObject> create = std::make_unique<GameObject>();
	GameObject* retPtr = create.get();
	*retPtr = _obj;

	// �������W�ݒ�
	create->transform.position = _worldPos;

	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();
	// �V�[���I�u�W�F�N�g�z��ɒǉ�
	sceneObjects.SetObject(retPtr->GetName(), std::move(create));

	return *retPtr;
}
