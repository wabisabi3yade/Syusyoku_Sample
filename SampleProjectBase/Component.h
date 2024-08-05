#pragma once
class GameObject;

// �Q�[���̃T�u�V�X�e���ƂȂ�R���|�[�l���g�̊��N���X
class Component
{
	friend class GameObject;

protected:
	// �����t���O
	bool isEnable;

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

	// �I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform& GetTransform() const;

	// ImGui�Ńp�����[�^��ς��鏈��
	virtual void ImGuiSetting() {};	

	const GameObject& GetGameObject() { return *gameObject; }	// �Q�[���I�u�W�F�N�g�擾
};