#pragma once

class AssetCollection;

// �A�Z�b�g�Ǘ����Z�b�g�ł���N���X���w��
class MainApplication;

// �A�Z�b�g�̌^��
class Texture;
class Mesh_Group;
class StaticMesh;
class SkeletalMesh;
class BoneList;
class Material;
class AnimationData;

// ���{����̖��O���
namespace HashiTaku	
{
	// �R���Z�v�g�Ŏ擾�ł��郊�\�[�X�̌^���w��
	template<typename T>
	concept AssetType =
		std::is_same_v<T, Texture> || std::is_same_v<T, Mesh_Group> ||
		std::is_same_v<T, StaticMesh> || std::is_same_v<T, SkeletalMesh> || 
		std::is_same_v<T, BoneList> || std::is_same_v<T, Material> ||
		std::is_same_v<T, AnimationData>;
}

// �A�Z�b�g�Ǘ��ɃA�N�Z�X����N���X�̊��N���X
class AssetContacter
{
	friend class MainApplication;

	/// @brief �A�Z�b�g�Ǘ��N���X��ݒ�
	/// @param _assetCollection �A�Z�b�g�Ǘ��N���X�̎Q��
	static void SetAssetCollection(AssetCollection& _assetCollection);
protected:
	/// @brief �A�Z�b�g�Ǘ��̃N���X
	static AssetCollection* pAssetCollection;	
};