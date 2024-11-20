#pragma once
// �A�Z�b�g�R���Z�v�g
#include "AssetConcept.h"

// �����A�Z�b�g�̌^
class Texture;
class Mesh_Group;
class SkeletalMesh;
class StaticMesh;
class Material;
class AnimationData;
class VisualEffect;
class BoneList;
class AnimationController;

// �A�Z�b�g�Ǘ��ɃA�N�Z�X����N���X
class AssetLoader;
class AssetGetter;
class AssetDisplay;
class AssetSetter;
class AssetSaveLoader;

// �A�Z�b�g�̊Ǘ��z��
using AssetList = std::unordered_map<std::string, std::unique_ptr<Asset_Base>>;

// �Q�[�����ł̗��p����A�Z�b�g���Ǘ�����N���X
// �Ǘ��N���X�̓��[�U�[����A�N�Z�X�ł��Ă��܂��ƃA�Z�b�g�𖳑���ɓ����ꂽ�肷��
// �̂Ō��߂��N���X���炵���A�N�Z�X�ł��Ȃ��悤�ɂ���
class AssetCollection
{
	// �A�N�Z�X�ł���N���X
	friend class AssetLoader;
	friend class AssetGetter;
	friend class AssetDisplay;
	friend class AssetSetter;
	friend class AssetSaveLoader;

	/// @brief �e�N�X�`���̃A�Z�b�g���X�g
	AssetList textureAssets;

	/// @brief �X�^�e�B�b�N�E�X�P���^�����b�V���̃A�Z�b�g���X�g
	AssetList modelAssets;

	/// @brief �}�e���A���̃A�Z�b�g���X�g
	AssetList materialAssets;

	/// @brief �G�t�F�N�g�̃A�Z�b�g���X�g
	AssetList vfxAssets;

	/// @brief �A�j���[�V�����̃A�Z�b�g���X�g
	AssetList animationAssets;

	/// @brief �{�[���̃A�Z�b�g���X�g
	AssetList boneAssets;

	/// @brief �A�j���[�V�����R���g���[���[
	AssetList animControllerAssets;

	/// @brief �A�Z�b�g��ǉ�����
	/// @tparam T �A�Z�b�g�̌^��
	/// @param _assetName �A�Z�b�g�̖��O
	/// @param _assetPtr �A�Z�b�g�̃��j�[�N�|�C���^
	/// @return �A�Z�b�g�̃|�C���^
	template<class T> T* SetAsset(const std::string& _assetName, std::unique_ptr<T> _assetPtr);

	/// @brief �A�Z�b�g���擾����
	/// @tparam T �A�Z�b�g�̌^��
	/// @param _assetName �A�Z�b�g�̖��O
	/// @return �A�Z�b�g�̃|�C���^
	template<class T> T* GetAsset(const std::string& _assetName);

	/// @brief �A�Z�b�g���폜����
	/// @tparam T �A�Z�b�g�̌^��
	/// @param _assetName �A�Z�b�g�̖��O
	template<class T> void DeleteAsset(const std::string& _assetName);
public:
	AssetCollection() {}
	~AssetCollection() {}

	/// @brief �A�Z�b�g���C���|�[�g����Ă��邩�m�F
	/// @tparam T �A�Z�b�g�̌^��
	/// @param _assetName �A�Z�b�g�̖��O
	/// @return �A�Z�b�g���C���|�[�g���Ă��邩�ǂ���
	template<class T> bool CheckImport(const std::string& _assetName);

private:
	/// @brief �ǂ̔z������o����
	/// @tparam T �A�Z�b�g�̃^�C�v
	/// @return �A�Z�b�g�̔z��
	template<class T> AssetList& GetAssetList();
};

/// @brief �A�Z�b�g���C���|�[�g����Ă��邩�m�F
/// @tparam T �A�Z�b�g�̌^��
/// @param _assetName �A�Z�b�g�̖��O
/// @return �A�Z�b�g���C���|�[�g���Ă��邩�ǂ���
template<class T>
inline bool AssetCollection::CheckImport(const std::string& _assetName)
{
	// �Ή������z������o��
	AssetList& assetList = GetAssetList<T>();

	// ��������
	auto itr = assetList.find(_assetName);

	// �Ōゾ�ƃC���|�[�g����Ă��Ȃ�
	return itr != assetList.end() ? true : false;
}

/// @brief �A�Z�b�g��ǉ�����
/// @tparam T �A�Z�b�g�̌^��
/// @param _assetName �A�Z�b�g�̖��O
/// @param _assetPtr �A�Z�b�g�̃��j�[�N�|�C���^
/// @return �A�Z�b�g�̃|�C���^
template<class T>
inline T* AssetCollection::SetAsset(const std::string& _assetName, std::unique_ptr<T> _assetPtr)
{
	AssetList& assetList = GetAssetList<T>();

	// �����A�Z�b�g���C���|�[�g���Ă��Ȃ炻�̃A�Z�b�g��Ԃ�
	if (CheckImport<T>(_assetName))
		return static_cast<T*>(assetList[_assetName].get());

	Asset_Base& asset = static_cast<Asset_Base&>(*_assetPtr);
	asset.SetAssetName(_assetName);

	// �߂�l�̃|�C���^���擾
	T* pRetAsset = _assetPtr.get();
	assetList[_assetName] = std::move(_assetPtr);

	return pRetAsset;
}

/// @brief �A�Z�b�g���擾����
/// @tparam T �A�Z�b�g�̌^��
/// @param _assetName �A�Z�b�g�̖��O
/// @return �A�Z�b�g�̃|�C���^
template<class T>
inline T* AssetCollection::GetAsset(const std::string& _assetName)
{
	AssetList& assetList = GetAssetList<T>();

	// ���O����T��
	auto itr = assetList.find(_assetName);
	if (itr == assetList.end())
	{
		std::string message = "���[�h����Ă��Ȃ��̂Ŏ擾�ł��܂���@" + _assetName;
		HASHI_DEBUG_LOG(message);
		return nullptr;
	}

	// �|�C���^�����炤
	Asset_Base* basePtr = itr->second.get();
	T* retPtr = static_cast<T*>(basePtr);

	return retPtr;	
}

template<class T>
inline void AssetCollection::DeleteAsset(const std::string& _assetName)
{
	AssetList& assetList = GetAssetList<T>();

	assetList.erase(_assetName);
}

/// @brief �ǂ̔z������o����
/// @tparam T �A�Z�b�g�̃^�C�v
/// @return �A�Z�b�g�̔z��
template<class T>
inline AssetList& AssetCollection::GetAssetList()
{
	// �e�N�X�`��
	if constexpr (std::is_same<T, Texture>::value)
		return textureAssets;

	// �X�^�e�B�b�N�E�X�P���^�����b�V�����b�V��
	else if constexpr (std::is_same<T, Mesh_Group>::value || std::is_same<T, SkeletalMesh>::value ||
		std::is_same<T, StaticMesh>::value)
		return modelAssets;

	// �}�e���A��
	else if constexpr (std::is_same<T, Material>::value)
		return materialAssets;

	// �G�t�F�N�g
	else if constexpr (std::is_same<T, VisualEffect>::value)
		return vfxAssets;

	// �A�j���[�V����
	else if constexpr (std::is_same<T, AnimationData>::value)
		return animationAssets;

	else if constexpr (std::is_same<T, BoneList>::value)
		return boneAssets;

	else if constexpr (std::is_same<T, AnimationController>::value)
		return animControllerAssets;

	assert(!"�A�Z�b�g�z��擾�ŕs���Ȍ^���ł�");
	return textureAssets;
}
