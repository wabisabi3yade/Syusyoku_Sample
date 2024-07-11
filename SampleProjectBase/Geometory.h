#pragma once
#include "Cube.h"
#include "DebugCube.h"
#include "Sphere.h"

class Material;

class Geometory
{	
	// �g�����X�t�H�[��
	static Transform transform;

	// Material
	static Material* pMaterial;

	// �F
	static DirectX::SimpleMath::Color color;	

	// �L���[�u
	static Cube* pCube;	
	
	// ��
	static Sphere* pSphere;	

	// �f�o�b�O�̃L���[�u 
	static std::unique_ptr<DebugCube> pDebugCube;


	Geometory() {};
	~Geometory() {};

	static void DrawSetup();
public:
	static void Init();	// ������
	static void Release();

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

	static void DrawCube(bool _isWireFrame = false);	// �L���[�u�`��

	static void DrawSphere(bool _isWireFrame = false);	// ���`��
};

