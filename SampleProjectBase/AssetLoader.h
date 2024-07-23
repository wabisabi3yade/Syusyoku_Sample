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
#include "AssetSetter.h"
#include "AssetGetter.h"

// �A�Z�b�g�̑O���錾
class Texture;
class Mesh_Group;
class SingleMesh;
class Material;
class AnimationData;

// �{�[�����
class Bone;

// assimp�\����
struct aiScene;
struct aiMesh;
struct  aiTexture;

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
	static void MaterialLoad(Mesh_Group* _pMeshgather, const aiScene* pScene, std::string texturedirectory);

	/// @brief ���f���̓����e�N�X�`����ǂݍ��ރN���X
	/// @param _texture ��������e�N�X�`���̎Q��
	/// @param _aiTex �����e�N�X�`��
	static bool LoadEmbeddedTexture(Texture& _texture, const aiTexture& _aiTex);

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
	static Mesh_Group* ModelLoad(const std::string& _modelPath, float _scale, bool _isLeftHand, 
		bool _isGetScale = true);


	/// @brief �A�j���[�V���������[�h���ăA�Z�b�g�Ǘ��ɒǉ�
	/// @param _animPath �A�j���[�V�����̃p�X
	/// @param _isLeftHand ����n���H
	/// @return �A�j���[�V�����f�[�^�N���X
	static AnimationData* AnimationLoad(const std::string& _animPath, bool _isLeftHand);

	

private:	// �֗��֐�

	/// @brief �X�^�e�B�b�N���X�P���^���𔻒f���č쐬����
	/// @param _pScene �V�[�����
	/// @return �쐬�������b�V���Q
	static std::unique_ptr<Mesh_Group> CreateMeshGroup(const aiScene* _pScene);

	/// @brief �{�[������ǂݍ���
	/// @param _pAiScene �V�[�����
	/// @param _skeletalMesh �X�P���^�����b�V��
	static void CreateBone(const aiScene* _pAiScene, SkeletalMesh& _skeletalMesh);

	/// @brief ���b�V������{�[�������擾
	/// @param _pAiMesh ai���b�V��
	/// @return ���b�V���̃{�[�����
	
	/// @brief ���b�V������{�[�������擾
	/// @param _pAiMesh ai���b�V��
	/// @param _singleMesh ����b�V��
	/// @param _boneIdx �{�[���C���f�b�N�X
	/// @return ���b�V���̃{�[�����
	static std::vector<std::unique_ptr<Bone>> CreateBone(const aiMesh* _pAiMesh, SingleMesh& _singleMesh, u_int& _boneIdx);

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
	if (AssetSetter::CheckImport<T>(_assetName))
	{
		return AssetGetter::GetAsset<T>(_assetName);
	}

	return AssetSetter::SetAsset(_assetName, std::move(_pAsset));
}
