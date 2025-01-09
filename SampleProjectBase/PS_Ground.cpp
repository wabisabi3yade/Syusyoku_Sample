#include "pch.h"
#include "PS_Ground.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	PS_Ground::PS_Ground()
	{
	}

	void PS_Ground::OnceUpdateBuffer()
	{
		// 深度テクスチャ
		Texture& texture = InSceneSystemManager::GetInstance()->GetShadowDrawer().GetDepthRTV();
		SetTexture(1, &texture);
	}
}
