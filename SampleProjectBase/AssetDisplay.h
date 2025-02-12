#pragma once
#include "AssetContacter.h"

namespace HashiTaku
{
	// アセットのインポートされたものを表示する
	class AssetDisplay : private AssetContacter
	{
		AssetDisplay() {}
		~AssetDisplay() {}

		static char inputText[IM_INPUT_BUF];
	public:

		/// @brief アセットの中身を描画
		static void Draw();

	private:
		static void Display(const std::map<std::string, std::unique_ptr<Asset_Base>>& _assets);
		static void DisplayTexture();
		static void DisplayModel();
		static void DisplayMaterial();
		static void DisplayVFX();
		static void DisplayAnimation();
		static void DisplayBoneList();
		static void DisplayAnimationController();
		static void DisplaySound();
		static void DeleteInputAsset();
	};
}