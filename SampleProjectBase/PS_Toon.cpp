#include "pch.h"
#include "PS_Toon.h"
#include "InSceneSystemManager.h"
#include "SceneLights.h"
#include "CP_Camera.h"

constexpr u_int LIGHT_SLOT(1);

PS_Toon::PS_Toon()
{
	bufferTypes.resize(2);
	bufferTypes[0] = BufferType::Material;
	bufferTypes[LIGHT_SLOT] = BufferType::Light;
}

void PS_Toon::OnceUpdateBuffer()
{
	auto pInSceneManager = InSceneSystemManager::GetInstance();

	// ライトの更新
	SceneLightsParam& lightParam = pInSceneManager->GetSceneLights().GetLightsParam();
	UpdateSubResource(LIGHT_SLOT, &lightParam.dirParam);
}
