#pragma once
#include "GameObject.h"
#include "Model.h"

// 3D���f�����������I�u�W�F�N�g
class Object_3D : public GameObject
{
	std::unique_ptr<Model> pModel;	// ���f�����
	const Model* pConstModel;	// ���_���W�Ȃǂ��ς��Ȃ��Ȃ�const�|�C���^�ł��炢�A�������ߖ�

	void Release();	// �������
public:
	Object_3D();
	virtual ~Object_3D();

	void SetModel(const Model* _setModel) { pConstModel = _setModel; }

	void Update() override;	// �X�V����
	void Draw() override;	// �`�揈��
};

