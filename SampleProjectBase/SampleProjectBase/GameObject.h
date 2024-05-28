#pragma once
#include "Model.h"

// ��ʂɎʂ����I�u�W�F�N�g�N���X
class GameObject
{
	std::unique_ptr<Model> pModel;	// ���f�����
	const Model* pConstModel;	// ���_���W�Ȃǂ��ς��Ȃ��Ȃ�const�|�C���^�ł��炢�A�������ߖ�

	void Release();	// �������
public:
	Transform transform;	// �ړ����

	GameObject();
	virtual ~GameObject();

	void SetModel(const Model* _setModel) { pConstModel = _setModel; }

	void Update();	// �X�V����
	void Draw();	// �`�揈��
};

