#pragma once
#include "GameObject.h"

class Camera;
// �V�[���Ŏg�p����I�u�W�F�N�g�z��N���X
class SceneObjects
{
	// �V�[���Ŏg�p����I�u�W�F�N�g�z��i�I�u�W�F�N�g�̖��O���L�[�l�j
	std::unordered_map<std::string, std::unique_ptr<GameObject>> list;
	// UI�p�̃��X�g(�`�����̃��X�g����ɂ��邽��)
	std::unordered_map<std::string, std::unique_ptr<GameObject>> uiList;
public:
	SceneObjects();
	~SceneObjects();

	void Update();	// �X�V����
	void LateUpdate();	// Update�̂��Ƃ̍X�V����
	void Draw();	// �`�揈��(�J�����Ƃ̋����ŕ`�揇�����߂�)

	// �z��ɃI�u�W�F�N�g������
	void SetObject(const std::string& _objectName, std::unique_ptr<GameObject> _objPtr);

	// �I�u�W�F�N�g�̖��O����I�u�W�F�N�g��Ԃ�
	template<class T> T* GetSceneObject(const std::string& _objectName);

	// �I�u�W�F�N�g�̐���Ԃ�
	u_int GetObjectsNum()const { return static_cast<u_int>(list.size()); }
};

template<class T>
inline T* SceneObjects::GetSceneObject(const std::string& _objectName)
{
	// ���3D��ԏ�̃I�u�W�F�N�g����T���@���@����������UI�̕���T��
	
	// ���O����T��
	auto itr = list.find(_objectName);
	if (itr == list.end())
	{
		// �I�u�W�F�N�g�ɖ���������Ui�ŒT��
		itr = uiList.find(_objectName);

		// ����ł��Ȃ�������
		if (itr == list.end())
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
