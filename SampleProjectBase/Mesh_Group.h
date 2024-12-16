#pragma once
#include "AssetPath_Base.h"

// �`��֘A
#include "SingleMesh.h"
#include "Material.h"
#include "Texture.h"

namespace HashiTaku
{
	/// @brief ���b�V���Q�i�X�P���^���E�X�^�e�B�b�N���b�V���j�̊��N���X
	class Mesh_Group : public AssetPath_Base
	{
	public:

		/// @brief ���b�V���̎��
		enum class MeshType
		{
			SM,	// �X�^�e�B�b�N���b�V��
			SK,	// �X�P���^�����b�V��
			None	// �ݒ�Ȃ�
		};
	private:
		/// @brief ���b�V���z��
		std::vector<std::unique_ptr<SingleMesh>> pMeshes;

		/// @brief �}�e���A���z��
		std::vector<Material*> materials;

		/// @brief ���S���W
		DirectX::SimpleMath::Vector3 centerPos;

		/// @brief ���f���T�C�Y
		DirectX::SimpleMath::Vector3 size;

		/// @brief ���[�h���A�X�P�[���{��
		float loadScale;

		/// @brief ���[�h���I�t�Z�b�g�p�x
		DirectX::SimpleMath::Vector3 loadOffsetAngles;

		/// @brief ���[�h��Y�����]�������H
		bool isFlipY;

		/// @brief ���b�V���̎��
		MeshType meshType;

		/// @brief �T�C�Y�͎擾���邩�H
		bool isGetSize;

		/// @brief �E��n�̃��f�����H
		bool isRightHand;
	public:
		Mesh_Group() : meshType(MeshType::None), loadScale(1.0f), isFlipY(false), isGetSize(false), isRightHand(false) {}

		/// @brief �R���X�g���N�^
		/// @param _meshType ���b�V���̎��
		Mesh_Group(MeshType _meshType) : meshType(_meshType), loadScale(1.0f), isFlipY(false), isGetSize(false), isRightHand(false) {}
		virtual ~Mesh_Group() {}

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
		SingleMesh* GetMesh(u_int _meshIdx) const;

		/// @brief ���b�V���̐��擾
		/// @return ���b�V���̐�
		u_int GetMeshNum();

		/// @brief �}�e���A���擾mesh
		/// @param _materialIdx �}�e���A��ID
		/// @return �}�e���A���̃|�C���^
		Material* GetMaterial(u_int _materialIdx);

		/// @brief ���_�V�F�[�_�[�擾
		/// @return ���_�V�F�[�_�[
		VertexShader* GetVertexShader();

		/// @brief �}�e���A���̐��擾
		/// @return �}�e���A���̐�
		u_int GetMaterialNum();

		/// @brief ���S���W���擾
		/// @return ���S���W
		DirectX::SimpleMath::Vector3 GetCenterPosition() const;

		/// @brief ���f���̃T�C�Y���擾
		/// @return �T�C�Y
		DirectX::SimpleMath::Vector3 GetSize() const;

		// ���[�h���̃X�P�[���{�����擾����
		float GetLoadOffsetScale() const;

		/// ���[�h���̃I�t�Z�b�g�p�x���擾����
		DirectX::SimpleMath::Vector3 GetLoadOffsetAngles() const;

		/// @brief ���b�V���Q�̎�ނ��擾
		/// @return ���
		MeshType GetType() const;

		/// @brief ���f���̒��S���W���Z�b�g
		/// @param _size ���S���W
		void SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos);

		/// @brief �T�C�Y���Z�b�g
		/// @param _size �T�C�Y
		void SetSize(const DirectX::SimpleMath::Vector3& _size);

		/// @brief �X�P�[���{�����Z�b�g
		/// @param _scaleTimes �X�P�[���{��
		void SetLoadOffsetScale(float _scaleTimes);

		/// @brief Y�����]���������Z�b�g
		/// @param  _isLoadFlipY Y�����]�������H
		void SetLoadFlipY(bool _isLoadFlipY);

		// �E��n���ǂ����Z�b�g
		void SetIsRightHand(bool _isRightHand);

		// �T�C�Y���擾���邩�Z�b�g
		void SetIsGetSize(bool _isGetSize);

		// �}�e���A���̒��_�E�s�N�Z���V�F�[�_�[���Z�b�g
		void SetVertexShader(const std::string& _vsName);
		void SetPixelShader(const std::string& _psName);

		/// @brief �Z�[�u����
		/// @param _sceneData �Z�[�u�f�[�^
		nlohmann::json Save() override;

		/// @brief ���[�h����
		/// @param _sceneData ���[�h����f�[�^ 
		void Load(const nlohmann::json& _data) override;
	};
}