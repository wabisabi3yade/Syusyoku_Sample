#pragma once

class AssetCollection;

// �A�Z�b�g�̌^��
class Texture;
class StaticMesh;
class SkeletalMesh;
class Material;

// ���{����̖��O���
namespace HashiTaku	
{
	// �R���Z�v�g�Ŏ擾�ł��郊�\�[�X�̌^���w��
	template<typename T>
	concept AssetType =
		std::is_same_v<T, Texture> || std::is_same_v<T, StaticMesh> ||
		std::is_same_v<T, SkeletalMesh> || std::is_same_v<T, Material>;
}

// �A�Z�b�g�Ǘ��ɃA�N�Z�X����N���X�̊��N���Xs
class AssetContacter
{
protected:
	/// @brief �A�Z�b�g�Ǘ��̃N���X
	static AssetCollection* pAssetCollection;

public:
	/// @brief �A�Z�b�g�Ǘ��N���X��ݒ�
	/// @param _assetCollection �A�Z�b�g�Ǘ��N���X�̎Q��
	static void SetAssetCollection(AssetCollection& _assetCollection);
};