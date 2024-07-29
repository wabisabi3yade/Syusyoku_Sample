#include "pch.h"
#include "PS_Unlit.h"

#include "InSceneSystemManager.h"

constexpr u_int BUFFER_NUM(2);
constexpr u_int MATERIAL_SLOT(0);
constexpr u_int LIGHT_SLOT(1);

PS_Unlit::PS_Unlit()
{
	bufferTypes.resize(BUFFER_NUM);
	bufferTypes[MATERIAL_SLOT] = BufferType::Material;
	bufferTypes[LIGHT_SLOT] = BufferType::Light;
}

void PS_Unlit::OnceUpdateBuffer()
{
	InSceneSystemManager* pSceneSystem = InSceneSystemManager::GetInstance();

	// ライトの更新
	SceneLightsParam& lightParam = pSceneSystem->GetSceneLights().GetLightsParam();
	UpdateSubResource(LIGHT_SLOT, &lightParam);
}
