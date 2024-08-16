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
	// Material
	static Material* pMaterial;

	/// @brief ���[���h���W
	static DirectX::SimpleMath::Vector3 position;

	/// @brief �傫��
	static DirectX::SimpleMath::Vector3 scale;

	/// @brief �p�x
	static DirectX::SimpleMath::Vector3 eularAngle;

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
		position = _position;
	}
	// ��]��ݒ�
	static void SetRotation(const DirectX::SimpleMath::Vector3& _eularAngle)
	{
		eularAngle = _eularAngle;
	}
	// �傫����ݒ�
	static void SetScale(const DirectX::SimpleMath::Vector3& _scale)
	{
		scale = _scale;
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

	/// @brief �p�����[�^�����Z�b�g
	static void ResetParameter();
};
