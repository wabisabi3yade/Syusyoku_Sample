/* --------------------------------------------------------------------------
Assimp�Ǎ��̏����\����HAL���̉ȖځFGM31�̃T���v�����Q�l�ɂ��č���Ă��܂�

2024/07/08
HAL���@
���{���
----------------------------------------------------------------------------*/

#pragma once
#include "AssetContacter.h"

// �A�Z�b�g
#include "AssetCollection.h"

// �A�Z�b�g�̑O���錾
class Texture;
class Mesh_Base;
class Material;

// �A�Z�b�g�����[�h����N���X(static�N���X)
class AssetLoader : private AssetContacter
{
	AssetLoader() {};
	~AssetLoader() {};

	/// @brief �A�Z�b�g�Ǘ��N���X�ɃA�Z�b�g�𑗂�
	/// @tparam T �A�Z�b�g�̌^
	/// @param _assetName �A�Z�b�g�̖��O
	/// @param _pAsset �A�Z�b�g�̃��j�[�N�|�C���^
	/// @return �A�Z�b�g�̃|�C���^
	template<class T> static T* SendAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset);

	/// @brief �}�e���A���Ǎ�
	/// @param _pMeshgather ���b�V��
	/// @param pScene �V�[��
	/// @param texturedirectory �ǂݍ��ރ��f���܂ł̃p�X��
	static void MaterialLoad(Mesh_Base* _pMeshgather, const aiScene* pScene, std::string texturedirectory);

	/// @brief ���f���̓����e�N�X�`����ǂݍ��ރN���X
	/// @param _texture ��������e�N�X�`���̎Q��
	/// @param Data �擪�A�h���X
	/// @param len �e�N�X�`���T�C�Y�i�������ɂ���ꍇ�A�����T�C�Y�j
	static bool TextureMemoryLoad(Texture& _texture , const unsigned char* Data, int len);

	/// @brief �e�N�X�`�����쐬����
	/// @param _filePath �e�N�X�`���p�X��
	/// @return �e�N�X�`���̃��j�[�N�|�C���^
	static std::unique_ptr<Texture> MakeTexture(const std::string& _filePath);
public:
	
	/// @brief �e�N�X�`�������[�h���ăA�Z�b�g�Ǘ��ɒǉ�����
	/// @param _filePath �e�N�X�`���̃p�X
	/// @return �����������t���O
	static Texture* TextureLoad(const std::string& _filePath);
	
	/// @brief  ���f�������[�h���ăA�Z�b�g�Ǘ��ɒǉ�
	/// @param _modelPath ���f���̃p�X��
	/// @param _scale ���[�h����Ƃ��̃X�P�[���l
	/// @param _isRightHand ����n���H
	/// @param _isGetScale ���f���̃X�P�[�����擾���邩�H
	/// @return ���[�h�������b�V��
	static Mesh_Base* ModelLoad(const std::string& _modelPath, float _scale, bool _isLeftHand, 
		bool _isGetScale = true);

private:	// �֗��֐�

	/// @brief �p�X����t�@�C�������擾����
	/// @param _pathName �p�X��
	///  @param _isExtexsion �g���q�����邩�H
	/// @return �t�@�C����
	static std::string PathToFileName(const std::string& _pathName, bool _isExtension);

	/// @brief �e�p�X�����擾
	/// @param _pathName �p�X��
	/// @return �e�p�X��
	static std::string GetParentPath(const std::string& _pathName);

	/// @brief �ő�E�ŏ��T�C�Y���X�V
	/// @param _vertexPos ���_���W
	/// @param _max �ő�
	/// @param _min �ŏ�
	static void UpdateSize(const DirectX::SimpleMath::Vector3& _vertexPos, DirectX::SimpleMath::Vector3& _max, DirectX::SimpleMath::Vector3& _min);
};

/// @brief �A�Z�b�g�Ǘ��N���X�ɃA�Z�b�g�𑗂�
/// @tparam T �A�Z�b�g�̌^
/// @param _assetName �A�Z�b�g�̖��O
/// @param _pAsset �A�Z�b�g�̃��j�[�N�|�C���^
template<class T>
inline T* AssetLoader::SendAsset(const std::string& _assetName, std::unique_ptr<T> _pAsset)
{
	// ���ɃC���|�[�g����Ă���Ȃ�
	if (pAssetCollection->CheckImport<T>(_assetName))
	{
		return pAssetCollection->GetAsset<T>(_assetName);
	}

	return pAssetCollection->SetAsset<T>(_assetName, std::move(_pAsset));
}
