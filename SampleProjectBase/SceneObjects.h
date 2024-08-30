#pragma once
#include "GameObject.h"
#include "Light.h"

class SceneFunction::ObjectFunc;

using Objects = std::unordered_map<std::string, std::unique_ptr<GameObject>>;

// �V�[���Ŏg�p����I�u�W�F�N�g�z��N���X
class SceneObjects
{
	friend class SceneFunction::ObjectFunc;

	// �V�[���Ŏg�p����I�u�W�F�N�g�z��i�I�u�W�F�N�g�̖��O���L�[�l�j
	Objects objList;

	// UI�p�̃��X�g(�`�����̃��X�g����ɂ��邽��)
	Objects uiList;

	// �z��ɃI�u�W�F�N�g������(ObjectFunc����A�N�Z�X)
	GameObject* SetObject(std::unique_ptr<GameObject> _objPtr);

public:
	SceneObjects();
	~SceneObjects();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();	

	void ImGuiSetting();

	//�@�z�񂩂�Q�[���I�u�W�F�N�g���폜����
	void DeleteObj(GameObject& _deleteObj);

	// �I�u�W�F�N�g�̖��O����I�u�W�F�N�g��Ԃ�
	GameObject* GetSceneObject(const std::string& _objectName);

	// �I�u�W�F�N�g�̐���Ԃ�
	u_int GetObjectsNum()const { return static_cast<u_int>(objList.size()); }

	// �V�[���I�u�W�F�N�g���Z�[�u�E���[�h
	nlohmann::json SaveObject();
	void LoadObject(const nlohmann::json& _objectsData);
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