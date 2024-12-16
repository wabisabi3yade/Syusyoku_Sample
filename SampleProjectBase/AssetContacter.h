#pragma once
#include "AssetConcept.h"

namespace HashiTaku
{
	class AssetCollection;

	// �A�Z�b�g�Ǘ����Z�b�g�ł���N���X���w��
	class MainApplication;

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
}