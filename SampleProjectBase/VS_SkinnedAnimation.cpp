#include "pch.h"
#include "VS_SkinnedAnimation.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	VS_SkinnedAnimation::VS_SkinnedAnimation()
	{
		bufferTypes.resize(3);
		bufferTypes[0] = BufferType::WVP;
		bufferTypes[1] = BufferType::BoneMatricies;
		bufferTypes[2] = BufferType::LightWVP;
	}

	void VS_SkinnedAnimation::OnceUpdateBuffer()
	{
		ShadowDrawer& shadowDraw = InSceneSystemManager::GetInstance()->GetShadowDrawer();
		// ライトビュー
		UpdateSubResource(2, &shadowDraw.GetLightMatrix());
	}
}