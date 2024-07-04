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

	void Update() override;

	void Draw() override;	// �`�揈��

	void SetModel(Model* _setModel) { pModel = _setModel; }

	Model& GetModel() { return *pModel; }
};

