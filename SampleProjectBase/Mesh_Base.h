#pragma once
#include "Asset_Base.h"

// �`��֘A
#include "SingleMesh.h"
#include "Material.h"
#include "Texture.h"

// ���f���i�X�P���^���E�X�^�e�B�b�N���b�V���j�̊��N���X
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
	std::vector<std::unique_ptr<SingleMesh>> pMeshes;

	/// @brief �}�e���A���z��
	std::vector<Material*> pMaterials;

	/// @brief ���S���W
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief ���f���T�C�Y
	DirectX::SimpleMath::Vector3 size;

	/// @brief ���b�V���̎��
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
	void AddMesh(std::unique_ptr<SingleMesh> _pMesh);

	/// @brief �}�e���A����ǉ�
	/// @param _pMaterial �Z�b�g����}�e���A��
	void AddMaterial(Material* _pMaterial);

	/// @brief ���b�V�����擾
	/// @param _meshIdx ����
	/// @return ���b�V��
	const SingleMesh* GetMesh(u_int _meshIdx) const;

	/// @brief ���b�V���̐��擾
	/// @return ���b�V���̐�
	u_int GetMeshNum();

	/// @brief �}�e���A���擾mesh
	/// @param _materialIdx �}�e���A��ID
	/// @return �}�e���A���̃|�C���^
	Material* GetMaterial(u_int _materialIdx);

	/// @brief ���S���W���擾
	/// @return ���S���W
	DirectX::SimpleMath::Vector3 GetCenterPosition() const;

	/// @brief ���f���̃T�C�Y���擾
	/// @return �T�C�Y
	DirectX::SimpleMath::Vector3 GetSize() const;

	/// @brief ���f���̒��S���W���Z�b�g
	/// @param _size ���S���W
	void SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos);

	/// @brief �T�C�Y���Z�b�g
	/// @param _size �T�C�Y
	void SetSize(const DirectX::SimpleMath::Vector3& _size);
};

