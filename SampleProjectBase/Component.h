#pragma once
#include "ISaveLoad.h"

class GameObject;

// �Q�[���̃T�u�V�X�e���ƂȂ�R���|�[�l���g�̊��N���X
class Component : public ISaveLoad
{
	friend class GameObject;
	
	// ���O
	std::string name;

	// �������
	bool isEnable;

	/// @brief Start�������s�������ǂ����H
	bool isAlreadyStart;
protected:
	// ���̃R���|�[�l���g�̏����I�u�W�F�N�g
	GameObject* gameObject;

public:
	Component() : name(""), isEnable(true), isAlreadyStart(false), gameObject(nullptr) {}
	virtual ~Component(){};

	/// @brief �쐬���ɌĂ΂�鏈��
	virtual void Awake() {}

	/// @brief �O������ŌĂяo��Start����
	void StartBase();

	// ���t���[���X�V����
	virtual void Update() {};	

	// Update�̌�ɍs���X�V����
	virtual void LateUpdate() {};

	// �`�揈��
	virtual void Draw() {};

	// ImGui�Ńp�����[�^��ς��鏈��
	virtual void ImGuiSetting() {};

	/// @brief ������Ԃ�؂�ւ���
	void TransitionEnable();

	/// @brief ���O���Z�b�g����
	/// @param _name ���O
	void SetName(const std::string& _name);

	// ������Ԃ��Z�b�g
	void SetEnable(bool _enable);

	// ���O���擾
	std::string GetName() const;

	// �I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform& GetTransform() const;

	// �Q�[���I�u�W�F�N�g�擾	
	GameObject& GetGameObject() const { return *gameObject; }

	// �����t���O�擾
	bool GetIsEnable() const;

	// Start�����s�������擾
	bool GetIsAlreadyStart() const;

	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�V�[���f�[�^
	virtual nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����V�[���f�[�^ 
	virtual void Load(const nlohmann::json& _sceneData) override;
	
protected:

	// 1�t���[���ڂōs������
	virtual void Start() {};
};
