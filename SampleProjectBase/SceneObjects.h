#pragma once
#include "GameObject.h"
#include "Light.h"

using Objects = std::unordered_map<std::string, std::unique_ptr<GameObject>>;

// �V�[���Ŏg�p����I�u�W�F�N�g�z��N���X
class SceneObjects
{
	// �V�[���Ŏg�p����I�u�W�F�N�g�z��i�I�u�W�F�N�g�̖��O���L�[�l�j
	Objects objList;

	// UI�p�̃��X�g(�`�����̃��X�g����ɂ��邽��)
	Objects uiList;

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
	GameObject* SetObject(std::unique_ptr<GameObject> _objPtr);

	//�@�z�񂩂�Q�[���I�u�W�F�N�g���폜����
	void DeleteObj(GameObject& _deleteObj);

	// �I�u�W�F�N�g�̖��O����I�u�W�F�N�g��Ԃ�
	template<class T> T* GetSceneObject(const std::string& _objectName);

	// �I�u�W�F�N�g�̐���Ԃ�
	u_int GetObjectsNum()const { return static_cast<u_int>(objList.size()); }

private:

	/// @brief �I�u�W�F�N�g�̖��O�����邩�m�F
	/// @param _gameObject �I�u�W�F�N�g
	void CheckEmptyName(GameObject& _gameObject);

	/// @brief �I�u�W�F�N�g�̖��O�����ɏd�����Ă��邩�m�F
	/// @param _gameObject 
	void CheckDuplicationName(GameObject& _gameObject, Objects& _objects);

	/// @brief UI���ǂ���
	/// @param _gameObject �Q�[���I�u�W�F�N�g 
	/// @return UI�t���O
	bool IsUI(GameObject& _gameObject);
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
			HASHI_DEBUG_LOG(message);
			return nullptr;
		}
	}

	// �w�肵���^�ɕϊ�����(GameObject��h�����Ă���N���X�̂�)
	T* retPtr = dynamic_cast<T*>(itr->second.get());
	if (retPtr == nullptr)	// GameObject��h�����Ă��Ȃ��N���X��������
	{
		std::string message = _objectName + "���w�肵���^�ɕϊ��ł��܂���ł���";
		HASHI_DEBUG_LOG(message);
		return nullptr;
	}

	return retPtr;
}
