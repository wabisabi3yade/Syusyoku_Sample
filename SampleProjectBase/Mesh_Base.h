#pragma once
#include "Asset_Base.h"

// �`��֘A
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

// ���b�V���W���́i�X�P���^���E�X�^�e�B�b�N���b�V���j�̊��N���X
class Mesh_Base : public Asset_Base
{
	/// @brief ���b�V���z��
	std::vector<std::unique_ptr<Mesh>> pMeshes;

	/// @brief �}�e���A���z��
	std::vector<Material*> pMaterials;
public:

	/// @brief �R���X�g���N�^
	/// @param _assetType �A�Z�b�g�̎��
	Mesh_Base() {}
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

