#include "pch.h"
#include "PS_Unlit.h"

#include "InSceneSystemManager.h"

namespace HashiTaku
{
	constexpr u_int BUFFER_NUM(2);	// �萔�o�b�t�@�̐�
	constexpr u_int MATERIAL_SLOT(0); // �}�e���A���̃X���b�g�ԍ�
	constexpr u_int LIGHT_SLOT(1); // ���C�g�p�����[�^�̃X���b�g�ԍ�

	constexpr u_int DEPTH_TEX_SLOT(1); // �[�x�e�N�X�`���̃X���b�g�ԍ�

	PS_Unlit::PS_Unlit()
	{
		bufferTypes.resize(BUFFER_NUM);
		bufferTypes[MATERIAL_SLOT] = BufferType::Material;
		bufferTypes[LIGHT_SLOT] = BufferType::Light;
	}

	void PS_Unlit::OnceUpdateBuffer()
	{
		InSceneSystemManager* pSceneSystem = InSceneSystemManager::GetInstance();

		// ���C�g�̍X�V
		SceneLightsParam& lightParam = pSceneSystem->GetSceneLights().GetLightsParam();
		UpdateSubResource(LIGHT_SLOT, &lightParam);

		// �[�x�e�N�X�`���̍X�V
		ShadowDrawer& shadowDraw = pSceneSystem->GetShadowDrawer();
		Texture& pDepthTex = shadowDraw.GetDepthRTV();
		SetTexture(DEPTH_TEX_SLOT, &pDepthTex);
	}
}