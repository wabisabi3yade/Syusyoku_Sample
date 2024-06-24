#pragma once
class GameObject;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// �Q�[���̃T�u�V�X�e���ƂȂ�R���|�[�l���g�̊��N���X
class Component
{
	bool isStartYet;	// �܂�Start�֐�����������Ă��Ȃ��t���O
protected:
	GameObject* gameObject;	// �R���|�[�l���g�������Ă���I�u�W�F�N�g

	Transform& GetTransform();	// �g�����X�t�H�[��
public:
	Component(GameObject* _gameObject) : gameObject(_gameObject), isStartYet(false) {};
	virtual ~Component(){};

	virtual void Init() {};	// AddComponent���ꂽ�Ƃ��ɍs��
	virtual void Start() {};	// �ŏ��̍X�V�t���[���ōs������
	virtual void Update() {};	// ���t���[���X�V����
	virtual void LateUpdate() {};	// Update�̌�ɍs���X�V����
	virtual void Draw() {};	// �`�揈��

	virtual void SetParameter() {};	// ImGui�Ńp�����[�^��ς��鏈��
	void StartComplete() { isStartYet = true; }	// Start�������I���������

	GameObject& GetGameObject() { return *gameObject; }	// �Q�[���I�u�W�F�N�g�擾
	bool GetIsStartYet() { return isStartYet; }
};

