#include "pch.h"
#include "VS_Gouraud.h"

// カメラ・ライトなど
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	constexpr u_int BUFFER_NUM(3);	// バッファの数
	constexpr u_int LIGHT_SLOT(2);	// ライトのスロット

	VS_Gouraud::VS_Gouraud()
	{
		bufferTypes.resize(BUFFER_NUM);
		bufferTypes[0] = BufferType::WVP;
		bufferTypes[1] = BufferType::Material;
		bufferTypes[2] = BufferType::Light;
	}

	void VS_Gouraud::OnceUpdateBuffer()
	{
		InSceneSystemManager* pSceneSystem = InSceneSystemManager::GetInstance();

		// ライトの更新
		SceneLightsParam& lightParam = pSceneSystem->GetSceneLights().GetLightsParam();
		UpdateSubResource(LIGHT_SLOT, &lightParam);
	}
}