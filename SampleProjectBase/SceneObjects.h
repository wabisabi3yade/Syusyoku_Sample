#pragma once
#include "GameObject.h"
#include "Light.h"

class SceneFunction::ObjectFunc;

using SceneObjectList = std::unordered_map<std::string, std::unique_ptr<GameObject>>;

// �V�[���Ŏg�p����I�u�W�F�N�g�z��N���X
class SceneObjects : public HashiTaku::IImGuiUser
{
	friend class SceneFunction::ObjectFunc;

	// �V�[���Ŏg�p����I�u�W�F�N�g�z��i�I�u�W�F�N�g�̖��O���L�[�l�j
	SceneObjectList objList;

	// UI�p�̃��X�g(�`�����̃��X�g����ɂ��邽��)
	SceneObjectList uiList;

	/// @brief �I�u�W�F�N�g�z���UI�z����s��������ׂɈꎞ�I�Ɋi�[���郊�X�g
	std::list<GameObject*> tmpMoveList;

	// �z��ɃI�u�W�F�N�g������(ObjectFunc����A�N�Z�X)
	GameObject* SetObject(std::unique_ptr<GameObject> _objPtr);

public:
	SceneObjects();
	~SceneObjects();

	/// @brief �I�u�W�F�N�gAwake����
	void Awake();

	/// @brief �I�u�W�F�N�gStart����
	void Start();

	/// @brief �I�u�W�F�N�gUpdate����
	void Update();

	/// @brief �I�u�W�F�N�gLateUpdate����
	void LateUpdate();

	/// @brief �I�u�W�F�N�gDraw����
	void Draw();

	//�@�z�񂩂�Q�[���I�u�W�F�N�g���폜����
	void DeleteGameObject(GameObject& _deleteObj);

	/// @brief UI�ƃI�u�W�F�N�g�̃��X�g�Ԃ��ړ�����
	/// @param _targetObj �ΏۃI�u�W�F�N�g
	void MoveTmpList(GameObject& _targetObj);

	// �I�u�W�F�N�g�̖��O����I�u�W�F�N�g��Ԃ�
	GameObject* GetSceneObject(const std::string& _objectName);

	// �I�u�W�F�N�g�̐���Ԃ�
	u_int GetObjectsNum()const { return static_cast<u_int>(objList.size()); }

	// �V�[���I�u�W�F�N�g���Z�[�u�E���[�h
	nlohmann::json SaveObjectList();
	void LoadObjectList(const nlohmann::json& _objectsData);
private:
	/// @brief �I�u�W�F�N�g�̖��O�����邩�m�F
	/// @param _gameObject �I�u�W�F�N�g
	void CheckEmptyName(GameObject& _gameObject);

	/// @brief �I�u�W�F�N�g�̖��O�����ɏd�����Ă��邩�m�F
	/// @param _gameObject 
	void CheckDuplicationName(GameObject& _gameObject, SceneObjectList& _objects);
	
	/// @brief �I�u�W�F�N�g�����o��
	/// @param _targetObject �Ώۂ̃I�u�W�F�N�g
	/// @param _objectName �I�u�W�F�N�g��
	/// @param _outObject �I�u�W�F�N�g�̃C���X�^���X
	/// @param _objectList ���o���ΏۃI�u�W�F�N�g���X�g
	/// @return �����������ǂ����H
	bool TakeOutObject(const GameObject& _targetObject, std::string& _objectName, std::unique_ptr<GameObject>& _outObject, SceneObjectList& _objectList);

	/// @brief UI���ǂ���
	/// @param _gameObject �Q�[���I�u�W�F�N�g 
	/// @return UI�t���O
	bool IsUI(GameObject& _gameObject);

	/// @brief UI�`��̑O�̏���
	void UIDrawSetup();

	/// @brief UI�`��I������
	void UIDrawEnd();

	/// @brief ���X�g�Ԃňړ�����I�u�W�F�N�g������Ȃ�ړ�����
	void MoveList();

	/// @brief UIList�Ɉړ�������
	/// @param _gameObject �Ώۂ̃Q�[���I�u�W�F�N�g
	void MoveToUIList(GameObject& _gameObject);

	/// @brief ObjList�Ɉړ�������
	/// @param _gameObject �Ώۂ�UI�I�u�W�F�N�g
	void MoveToObjList(GameObject& _gameObject);

	/// @brief �v���n�u�t�@�C���̃p�X�����擾
	/// @return �v���n�u�t�@�C���̃p�X��
	std::string PrefabFileParh();

	/// @brief �v���n�u�𐶐�
	/// @param _prefabName �v���n�u��
	void CreatePrefab(const std::string& _prefabName);

	void ImGuiDebug() override;

	/// @brief �I�u�W�F�N�g���Z�[�u
	/// @param _go �Q�[���I�u�W�F�N�g
	/// @return �Z�[�u�f�[�^
	nlohmann::json SaveObject(GameObject& _go);

	/// @brief �I�u�W�F�N�g���v���n�u������
	/// @param _toPrefabObject �v���n�u������I�u�W�F�N�g
	void ObjectToPrefab(GameObject& _toPrefabObject);

	/// @brief �I�u�W�F�N�g�����[�h����
	/// @param _gameObjectData �Q�[���I�u�W�F�N�g�f�[�^
	/// @return �����������H
	bool LoadObject(const nlohmann::json& _gameObjectData);

	/// @brief �I�u�W�F�N�g��x�ꂽ���[�h����
	/// @param _gameObjectData �Q�[���I�u�W�F�N�g�f�[�^
	/// @return �����������H
	bool LateLoadObject(const nlohmann::json& _gameObjectData);

	/// @brief �Q�[���I�u�W�F�N�g��ImGUi�\��
	/// @param _gameObject �\������Q�[���I�u�W�F�N�g
	/// @return delete������
	bool ImGuiSettingObject(GameObject& _gameObject);
};