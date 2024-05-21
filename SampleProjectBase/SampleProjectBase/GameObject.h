#pragma once
#include "Model.h"

// ��ʂɎʂ����I�u�W�F�N�g�N���X
class GameObject
{
	Model* model;	// ���f�����

	void Release();	// �������
public:
	Transform transform;	// �ړ����

	GameObject();
	virtual ~GameObject();

	void SetModel(Model* _setModel) { model = _setModel; }

	void Update();	// �X�V����
	void Draw();	// �`�揈��
};

