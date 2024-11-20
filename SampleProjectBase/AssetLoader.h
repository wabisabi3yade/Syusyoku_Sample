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
class SkeletalMesh;
class Material;
class AnimationData;

// �{�[���E�c���[�m�[�h
class Bone;
class TreeNode;
class AnimationChannel;

// assimp�\����
struct aiScene;
struct aiMesh;
struct aiTexture;
struct aiNode;
struct aiNodeAnim;
struct aiAnimation;

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
	static void MaterialLoad(Mesh_Group* _pMeshgather, const aiScene* pScene, const std::string& _texturedirectory);

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
	/// @param _isFlipY ��]����
	/// @param _isRightHand �E��n���H
	/// @param _isGetScale ���f���̃X�P�[�����擾���邩�H
	/// @return ���[�h�������b�V��
	static Mesh_Group* ModelLoad(const std::string& _modelPath, float _scale, bool _isFlipY, bool _isRightHand, bool _isGetScale = true);

	/// @brief �A�j���[�V���������[�h���ăA�Z�b�g�Ǘ��ɒǉ�
	/// @param _animPath �A�j���[�V�����̃p�X
	/// @param _boneName �Ή��{�[����
	/// @param _isLeftHand �E��n���H
	/// @return �A�j���[�V�����f�[�^�N���X
	static AnimationData* AnimationLoad(const std::string& _animPath, const std::string& _boneName,
		bool _isRightHand);

	/// @brief	�G�t�F�N�V�A��p�G�t�F�N�g�����[�h����
	/// @param _pathName �p�X��
	/// @param _loadScale ���[�h���̃X�P�[��
	/// @return ���[�h�����G�t�F�N�g
	static VisualEffect* VFXLoadForEffekseer(const std::string& _pathName, float _loadScale);

private:	// �֗��֐�

	/// @brief �X�^�e�B�b�N���X�P���^���𔻒f���č쐬����
	/// @param _pScene �V�[�����
	/// @param _assetName �A�Z�b�g��
	/// @param _loadScale ���[�h���̃X�P�[��
	/// @param _loadAngles ���[�h���̃I�t�Z�b�g�p�x
	/// @return �쐬�������b�V���Q
	static std::unique_ptr<Mesh_Group> CreateMeshGroup(const aiScene* _pScene, const std::string& _assetName, float _loadScale, const DirectX::SimpleMath::Vector3& _loadAngles);

	/// @brief �{�[���𐶐�����
	/// @param _pScene �V�[�����
	/// @param _skeletalMesh ��񂢂��X�P���^�����b�V��
	/// @param _loadScale ���[�h���̃X�P�[��
	/// @param _loadAngles ���[�h���̃I�t�Z�b�g�p�x
	static void CreateBone(const aiScene* _pScene, SkeletalMesh& _skeletalMesh, float _loadScale, const DirectX::SimpleMath::Vector3& _loadAngles);

	/// @brief �m�[�h�𐶐�����ċA�֐�
	/// @param _aiChildNode �q�m�[�h
	/// @param _skeletalMesh �X�P���^�����b�V��
	/// @return ���������m�[�h
	static std::unique_ptr<TreeNode> CreateNode(const aiNode& _aiChildNode, SkeletalMesh& _skeletalMesh);
	
	/// @brief	���_�ƃ{�[������R�Â���
	/// @param _pAiMesh ai���b�V��
	/// @param _singleMesh ��̃��b�V��
	/// @param _skeletalMesh�@�X�P���^�����b�V��
	static void LinkVertexToBone(const aiMesh* _pAiMesh, SingleMesh& _singleMesh, SkeletalMesh& _skeletalMesh);

	/// @brief �A�j���[�V�����m�[�h���쐬����
	/// @param _pAiAnimNode assimp�̃A�j���[�V�����m�[�h
	/// @param _animData �A�j���[�V�����m�[�h��ǉ�����A�j���[�V�����f�[�^
	/// @param _boneList �Ή��{�[�����X�g
	static std::unique_ptr<AnimationChannel> CreateAnimChannel(const aiNodeAnim* _pAiAnimNode, const BoneList& _boneList);

	/// @brief �A�j���[�V�����̎��Ԃ��擾����
	/// @param _pAiAnim assimp�A�j���[�V����
	/// @return �A�j���[�V��������
	static float GetAnimationTime(const aiAnimation* _pAiAnim);

	/// @brief �A�j���[�V�����̃L�[�����擾
	/// @param _pAiAnim assimp�A�j���[�V����
	/// @return �A�j���[�V�����L�[��
	static u_int GetAnimationFrameCnt(const aiAnimation* _pAiAnim);

	/// @brief 1�L�[���Ƃ̎��Ԃ��擾����
	/// @param _pAiAnim assimp�A�j���[�V����
	/// @return 1�L�[���Ƃ̎���
	static float GetTimePerKey(const aiAnimation* _pAiAnim);

	/// @brief �p�X����t�@�C�������擾����
	/// @param _pathName �p�X��
	/// @return �t�@�C����
	static std::string PathToFileName(const std::string& _pathName);

	/// @brief �e�p�X�����擾
	/// @param _pathName �p�X��
	/// @return �e�p�X��
	static std::string GetParentPath(const std::string& _pathName);

	/// @brief �g���q�����̃t�@�C�������擾����
	/// @param _pathName �p�X��
	/// @return �g�����Ȃ��t�@�C����
	static std::string GetPathNameNotExt(const std::string& _pathName);

	/// @brief string��u16string�ɕϊ�
	/// @param _string �ϊ�������string�^������
	/// @return u16string������
	static std::u16string ConvertToU16String(const std::string& _string);

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
