#pragma once

// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
class GameObject
{
protected:
	bool isActive;	// �A�N�e�B�u��Ԃ��ǂ���

public:
	Transform transform;	// Transform�p�����[�^

	GameObject() : isActive(true) {};
	virtual ~GameObject() {};

	virtual void Update(){};	// �X�V����
	virtual void LateUpdate() {};	// Update���s�������Ƃ̍X�V����
	virtual void Draw() {};	// �`�揈��
};

