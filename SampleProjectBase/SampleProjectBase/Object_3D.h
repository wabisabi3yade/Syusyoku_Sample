#pragma once
#include "GameObject.h"
#include "Model.h"

// 3D���f�����������I�u�W�F�N�g
class Object_3D : public GameObject
{
	Model* pModel;	// ���f��

	void Release();	// �������
public:
	Object_3D();
	virtual ~Object_3D();

	void SetModel(Model* _setModel) { pModel = _setModel; }

	void Update() override;	// �X�V����
	void LateUpdate() override;	// Update�̂��Ə���
	void Draw() override;	// �`�揈��

	Model& GetModel() { return *pModel; }
};

