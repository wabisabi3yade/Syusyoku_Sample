#pragma once
#include "Mesh.h"
#include "Material.h"

// �L���[�u�A���ȂǊ�{�I�ȃ��b�V���N���X�̊��
class BasicObject_Base : public Mesh
{
protected:
	Transform transform;
	const Material* material;	// �}�e���A��

	// ��{�I�u�W�F�N�g�̓}�e���A���łȂ�����ŐF�����߂�
	DirectX::SimpleMath::Color color;	

	virtual void Make() = 0;	// �e���b�V�����쐬
	
public:
	BasicObject_Base();
	virtual ~BasicObject_Base();

	virtual void Draw();	// �`��

	// �p�����[�^���
	void SetPosition(const DirectX::SimpleMath::Vector3& _position) { transform.position = _position; }
	void SetRotation(const DirectX::SimpleMath::Vector3& _rotation) { transform.position = _rotation; }
	void SetScale(const DirectX::SimpleMath::Vector3& _scale) { transform.position = _scale; }
	void SetTransform(const Transform& _transform) { transform = _transform; }
};