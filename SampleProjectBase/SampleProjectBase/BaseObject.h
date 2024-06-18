#pragma once
#include "Cube.h"

class BaseObject
{
	static Transform transform;	// �g�����X�t�H�[��
	static DirectX::SimpleMath::Color color;	// �F
	static std::unique_ptr<Cube> pCube;	// �L���[�u

	BaseObject() {};
	~BaseObject() {};
public:
	static void Init();	// ������

	// ���W��ݒ�
	static void SetPosition(const DirectX::SimpleMath::Vector3& _position)
	{
		transform.position = _position;
	}
	// ��]��ݒ�
	static void SetRotation(const DirectX::SimpleMath::Vector3& _rotation)
	{
		transform.rotation = _rotation;
	}
	// �傫����ݒ�
	static void SetScale(const DirectX::SimpleMath::Vector3& _scale)
	{
		transform.scale = _scale;
	}
	// �F
	static void SetColor(const DirectX::SimpleMath::Color& _color) { color = _color; }

	static void DrawCube();	// �L���[�u�`��
};

