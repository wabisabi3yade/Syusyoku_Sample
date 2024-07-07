#pragma once
#include "GameObject.h"
#include "Light.h"

class Camera;
// �V�[���Ŏg�p����I�u�W�F�N�g�z��N���X
class SceneObjects
{
	// �V�[���Ŏg�p����I�u�W�F�N�g�z��i�I�u�W�F�N�g�̖��O���L�[�l�j
	std::unordered_map<std::string, std::unique_ptr<GameObject>> objList;

	// UI�p�̃��X�g(�`�����̃��X�g����ɂ��邽��)
	std::unordered_map<std::string, std::unique_ptr<GameObject>> uiList;

public:
	SceneObjects();
	~SceneObjects();

	// �X�V����
	void Update();

	// Update�̂��Ƃ̍X�V����
	void LateUpdate();	

	// �`�揈��
	void Draw();	

	// �z��ɃI�u�W�F�N�g������
	GameObject* SetObject(const std::string& _objectName, std::unique_ptr<GameObject> _objPtr);

	//�@�z�񂩂�Q�[���I�u�W�F�N�g���폜����
	void DeleteObj(GameObject& _deleteObj);

	// �I�u�W�F�N�g�̖��O����I�u�W�F�N�g��Ԃ�
	template<class T> T* GetSceneObject(const std::string& _objectName);

	// �I�u�W�F�N�g�̐���Ԃ�
	u_int GetObjectsNum()const { return static_cast<u_int>(objList.size()); }
};

template<class T>
inline T* SceneObjects::GetSceneObject(const std::string& _objectName)
{
	// ���3D��ԏ�̃I�u�W�F�N�g����T���@���@����������UI�̕���T��
	
	// ���O����T��
	auto itr = objList.find(_objectName);
	if (itr == objList.end())
	{
		// �I�u�W�F�N�g�ɖ���������Ui�ŒT��
		itr = uiList.find(_objectName);

		// ����ł��Ȃ�������
		if (itr == uiList.end())
		{
			std::string message = "���X�g���ɖ��O�̃I�u�W�F�N�g������܂���ł���" + _objectName;
			ImGuiDebugLog::Add(message);
			return nullptr;
		}
	}

	// �w�肵���^�ɕϊ�����(GameObject��h�����Ă���N���X�̂�)
	T* retPtr = dynamic_cast<T*>(itr->second.get());
	if (retPtr == nullptr)	// GameObject��h�����Ă��Ȃ��N���X��������
	{
		std::string message = _objectName + "���w�肵���^�ɕϊ��ł��܂���ł���";
		ImGuiDebugLog::Add(message);
		return nullptr;
	}

	return retPtr;
}
