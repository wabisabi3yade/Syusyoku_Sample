#pragma once

// �`��
class StaticMesh;
class Material;

// �����蔻��̕`��Ȃǂ���y�ɂ���N���X(�f�o�b�O�p)
class Geometory
{	
public:

	/// @brief �}�`�̎��
	enum class GeoType
	{
		Cube,	// ������
		WireCube,	// ���C���[������
		Sphere	// ��
	};
private:
	// �g�����X�t�H�[��
	static Transform transform;

	// Material
	static Material* pMaterial;

	// �F
	static DirectX::SimpleMath::Color color;	

	// ��{�}�`
	static std::vector<StaticMesh*> pGeometory;	

	Geometory() {};
	~Geometory() {};

	// �`�揀��
	static void DrawSetup();

	// �`��
	static void Draw(StaticMesh& _staticMesh);
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

private:
	/// @brief �}�e���A�����쐬
	static void MakeMaterial();

	/// @brief ��ʓI�ȃ��b�V�����쐬
	static void MakeGeometory();
};

