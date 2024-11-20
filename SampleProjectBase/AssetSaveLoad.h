#pragma once
#include "AssetContacter.h"

// �A�Z�b�g�����Z�[�u���[�h����N���X
class AssetSaveLoader : public AssetContacter
{
	AssetSaveLoader() {}
	~AssetSaveLoader() {}

public:
	/// @brief �A�Z�b�g���Z�[�u
	/// @param _assetCollection �A�Z�b�g�R���N�V����
	static void Save();

	/// @brief �A�Z�b�g�����[�h
	/// @param _assetCollection �A�Z�b�g�R���N�V���� 
	static void Load();

private:
	/// @brief �Z�[�u����t�@�C���p�X���擾
	/// @return �t�@�C���p�X
	static std::string SaveFilePath();

	// ���[�h���̃A�Z�b�g�쐬
	static void CreateTexture(const nlohmann::json& _loadData);
	static void CreateMesh(const nlohmann::json& _loadData);
	static void CreateMaterial(const nlohmann::json& _loadData);
	static void CreateVFX(const nlohmann::json& _loadData);
	static void CreateAnimaton(const nlohmann::json& _loadData);
	static void CreateAnimController(const nlohmann::json& _loadData);

	// �A�Z�b�g�����[�h
	static void LoadAsset(const nlohmann::json& _loadData);
};
