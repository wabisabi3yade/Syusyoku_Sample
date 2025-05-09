#include "pch.h"
#include "VS_WorldPosition.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	VS_WorldPosition::VS_WorldPosition()
	{
		bufferTypes.push_back(BufferType::WVP);
		bufferTypes.push_back(BufferType::LightWVP);
	}

	void VS_WorldPosition::OnceUpdateBuffer()
	{
		auto& lightMtx = InSceneSystemManager::GetInstance()->GetShadowDrawer().GetLightMatrix();
		UpdateSubResource(1, &lightMtx);
	}
}