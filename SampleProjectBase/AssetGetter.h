#pragma once
#include "AssetContacter.h"

#include "AssetCollection.h"

namespace HashiTaku
{
	// �Ǘ��N���X������A�Z�b�g���擾����N���X
	class AssetGetter : private AssetContacter
	{
		AssetGetter() {}
		~AssetGetter() {}
	public:

		/// @brief ���O����A�Z�b�g���擾
		/// @tparam T �A�Z�b�g�̌^
		/// @param _assetName �A�Z�b�g��
		/// @return �A�Z�b�g�̃|�C���^
		template<AssetConcept T>
		static T* GetAsset(const std::string& _assetName);

		/// @brief �A�Z�b�g���C���|�[�g����Ă��邩�m�F
		/// @tparam T �A�Z�b�g�̌^
		/// @param _assetName �A�Z�b�g���O
		/// @return �C���|�[�g���Ă��邩�H
		template<AssetConcept T>
		static bool CheckImport(const std::string& _assetName);

		/*/// @brief ImGui����R���{�{�b�N�X�ŃA�Z�b�g���擾����
		/// @tparam T �A�Z�b�g�̌^
		/// @param _asset ����������A�Z�b�g�̕ϐ�
		/// @param _caption �L���v�V����
		/// @return �ύX�������H
		template<AssetConcept T>
		static bool ImGuiGetCombobox(T* _asset, const std::string& _caption);*/

		/// @brief ImGui����R���{�{�b�N�X�ŃA�Z�b�g���擾����
		/// @tparam T �A�Z�b�g�̌^
		/// @param _asset ����������A�Z�b�g�̕ϐ�
		/// @param _caption �L���v�V����
		/// @param _currentName ���ݑI��ł�
		/// @return �ύX�������H
		template<AssetConcept T>
		static bool ImGuiGetCombobox(const std::string& _caption, std::string& _currentName);
	};

	template<AssetConcept T>
	inline T* AssetGetter::GetAsset(const std::string& _assetName)
	{
		return pAssetCollection->GetAsset<T>(_assetName);
	}

	template<AssetConcept T>
	inline bool AssetGetter::CheckImport(const std::string& _assetName)
	{
		return pAssetCollection->CheckImport<T>(_assetName);
	}

	template<AssetConcept T>
	inline bool AssetGetter::ImGuiGetCombobox(const std::string& _caption, std::string& _currentName)
	{
		bool isChange = false;

#ifdef EDIT
		AssetList& assetList = pAssetCollection->GetAssetList<T>();
		std::vector<const std::string*> assetNames;
		for (auto& asset : assetList)
		{
			assetNames.push_back(&asset.first);
		}

		isChange = ImGuiMethod::ComboBox(_caption, _currentName, assetNames);
#endif // EDIT

		return isChange;
	}

	template<AssetConcept T> T* LoadJsonAsset(const std::string& _s, const json& _j);

	template<AssetConcept T>
	T* LoadJsonAsset(const std::string& _s, const json& _j)
	{
		if (!IsJsonContains(_j, _s)) return nullptr;

		if (!_j[_s].is_string())
		{
			HASHI_DEBUG_LOG(_s + "��string�ł͂���܂���");
			return nullptr;
		}

		return AssetGetter::GetAsset<T>(_j[_s]);
	}
}