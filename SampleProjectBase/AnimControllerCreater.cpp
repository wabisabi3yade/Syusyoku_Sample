#include "pch.h"
#include "AnimControllerCreater.h"

#include "AssetSetter.h"

namespace HashiTaku
{
	AnimationController& AnimControllerCreater::CraeteAsset(const std::string& _assetName)
	{
		std::unique_ptr<AnimationController> pCreate = std::make_unique<AnimationController>();

		return *AssetSetter::SetAsset<AnimationController>(_assetName, std::move(pCreate));
	}
}