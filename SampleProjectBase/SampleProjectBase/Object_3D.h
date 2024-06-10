#pragma once
#include "GameObject.h"
#include "Model.h"

// 3D���f�����������I�u�W�F�N�g
class Object_3D : public GameObject
{
	//std::unique_ptr<Model> pModel;	// ���f�����
	Model* pModel;

	void Release();	// �������
public:
	Object_3D();
	virtual ~Object_3D();

	void SetModel(Model* _setModel) { pModel = _setModel; }

	void Update() override;	// �X�V����
	void Draw() override;	// �`�揈��

	Model& GetModel() { return *pModel; }
};

