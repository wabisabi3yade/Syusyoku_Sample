#pragma once
#include "Mesh.h"
#include "MaterialClass.h"

// �L���[�u�A���ȂǊ�{�I�ȃ��b�V���N���X�̊��
class BasicObject_Base : public Mesh
{
protected:
	// Geomwtory����`�揈���ł̃}�e���A��
	Material* pBaseMaterial;

	// �}�e���A��
	Material* pMaterial;	

	virtual void Make() = 0;	// �e���b�V�����쐬
public:
	BasicObject_Base();
	virtual ~BasicObject_Base();

	virtual void BasicDraw(Transform& _transform, DirectX::SimpleMath::Color& _color);	// �`��
	void SetMaterial(Material& _material);

	void ImGuiSetting();
};