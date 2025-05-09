#include "pch.h"
#include "VS_Ground.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	VS_Ground::VS_Ground()
	{
	}

	void VS_Ground::OnceUpdateBuffer()
	{
		auto& lightMtx = InSceneSystemManager::GetInstance()->GetShadowDrawer().GetLightMatrix();
		UpdateSubResource(1, &lightMtx);
	}
}
