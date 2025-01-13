#include "pch.h"
#include "PS_Unlit.h"

#include "InSceneSystemManager.h"

namespace HashiTaku
{
	constexpr u_int BUFFER_NUM(2);	// 定数バッファの数
	constexpr u_int MATERIAL_SLOT(0); // マテリアルのスロット番号
	constexpr u_int LIGHT_SLOT(1); // ライトパラメータのスロット番号

	constexpr u_int DEPTH_TEX_SLOT(1); // 深度テクスチャのスロット番号

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

		// 深度テクスチャの更新
		ShadowDrawer& shadowDraw = pSceneSystem->GetShadowDrawer();
		Texture& pDepthTex = shadowDraw.GetDepthRTV();
		SetTexture(DEPTH_TEX_SLOT, &pDepthTex);
	}
}