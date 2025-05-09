#pragma once
#include "IAssetCreater.h"
#include "AnimationController.h"

namespace HashiTaku
{
	/// @brief アニメーションコントローラー作成クラス
	class AnimControllerCreater
	{
	public:
		AnimControllerCreater() {}
		~AnimControllerCreater() {}

		AnimationController& CraeteAsset(const std::string& _assetName);
	};
}