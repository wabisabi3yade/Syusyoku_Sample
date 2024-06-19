#pragma once
class GameObject;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// �Q�[���̃T�u�V�X�e���ƂȂ�R���|�[�l���g�̊��N���X
class Component
{
protected:
	GameObject* gameObject;	// �R���|�[�l���g�������Ă���I�u�W�F�N�g

public:
	Component(GameObject* _gameObject) : gameObject(_gameObject) {};
	virtual ~Component(){};

	virtual void Init() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void Draw() {};

	virtual void SetParameter() {};
};

