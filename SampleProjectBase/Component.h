#pragma once
class GameObject;

// �Q�[���̃T�u�V�X�e���ƂȂ�R���|�[�l���g�̊��N���X
class Component
{
	friend class GameObject;
	
	// �������
	bool isEnable;

protected:
	// ���O
	std::string name;	

	// ���̃R���|�[�l���g�̏����I�u�W�F�N�g
	GameObject* gameObject;

public:
	Component() : isEnable(true), name(""), gameObject(nullptr) {}
	virtual ~Component(){};

	// AddComponent���ꂽ�Ƃ��ɍs��
	virtual void Init() {};	

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

	// ���O���擾
	std::string GetName() const;

	// �I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform& GetTransform() const;

	// �Q�[���I�u�W�F�N�g�擾	
	const GameObject& GetGameObject() const { return *gameObject; }

	bool GetIsEnable() const;
};