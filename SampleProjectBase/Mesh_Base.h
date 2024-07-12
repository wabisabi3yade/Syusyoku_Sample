#pragma once
#include "Asset_Base.h"

// �`��֘A
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

// ���b�V���W���́i�X�P���^���E�X�^�e�B�b�N���b�V���j�̊��N���X
class Mesh_Base : public Asset_Base
{
public:

	/// @brief ���b�V���̎��
	enum class Type
	{
		SM,	// �X�^�e�B�b�N���b�V��
		SK,	// �X�P���^�����b�V��
		None	// �ݒ�Ȃ�
	};
private:
	/// @brief ���b�V���z��
	std::vector<std::unique_ptr<Mesh>> pMeshes;

	/// @brief �}�e���A���z��
	std::vector<Material*> pMaterials;

	Type meshType;
public:
	Mesh_Base() : meshType(Type::None) {}

	/// @brief �R���X�g���N�^
	/// @param _meshType ���b�V���̎��
	Mesh_Base(Type _meshType) : meshType(_meshType) {}
	virtual ~Mesh_Base() {}
	
	/// @brief ���b�V����ǉ�
	/// @param _pMesh ���b�V���̃��j�[�N�|�C���^
	/// @param _materialID �}�e���A���C���f�b�N�X
	void AddMesh(std::unique_ptr<Mesh> _pMesh);

	/// @brief �}�e���A����ǉ�
	/// @param _pMaterial �Z�b�g����}�e���A��
	void AddMaterial(Material& _pMaterial);

	/// @brief ���b�V���̐��擾
	/// @return ���b�V���̐�
	u_int GetMeshNum();
};

