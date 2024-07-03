#pragma once
#include "Mesh.h"
#include "MaterialClass.h"

// �L���[�u�A���ȂǊ�{�I�ȃ��b�V���N���X�̊��
class BasicObject_Base : public Mesh
{
protected:
	MaterialClass* pMaterial;	// �}�e���A��
	virtual void Make() = 0;	// �e���b�V�����쐬
	
public:
	BasicObject_Base();
	virtual ~BasicObject_Base();

	virtual void Draw(Transform& _transform, DirectX::SimpleMath::Color& _color);	// �`��
	void SetMaterial(MaterialClass& _material);
};