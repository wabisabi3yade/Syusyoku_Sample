#pragma once
#include "IAssetCreater.h"
#include "AnimationController.h"

namespace HashiTaku
{
	/// @brief �A�j���[�V�����R���g���[���[�쐬�N���X
	class AnimControllerCreater
	{
	public:
		AnimControllerCreater() {}
		~AnimControllerCreater() {}

		AnimationController& CraeteAsset(const std::string& _assetName);
	};
}