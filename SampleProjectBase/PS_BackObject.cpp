#include "pch.h"
#include "PS_BackObject.h"
#include "InSceneSystemManager.h"
#include "AssetLoader.h"

namespace HashiTaku
{
	PS_BackObject::PS_BackObject()
	{
		bufferTypes.resize(3);
		bufferTypes[0] = BufferType::Material;
		bufferTypes[1] = BufferType::Light;
		bufferTypes[2] = BufferType::CameraPos;
	}

	void PS_BackObject::InitAssets()
	{
		// �A���t�@�f�B�U�[�p�̃e�N�X�`�������[�h
		Texture* pDitherTex = AssetLoader::TextureLoad("assets/texture/shader/BayerMatrix.png");
		pDitherTex->SetIsntSave();

		SetTexture(1, pDitherTex);
	}

	void PS_BackObject::OnceUpdateBuffer()
	{
		InSceneSystemManager* pSceneSystem = InSceneSystemManager::GetInstance();

#ifdef EDIT
		if (pSceneSystem->GetIsNullCamera()) return;
#endif // EDIT

		// ���C�g�̍X�V
		SceneLightsParam& lightParam = pSceneSystem->GetSceneLights().GetLightsParam();
		UpdateSubResource(1, &lightParam);

		CP_Camera& cam = pSceneSystem->GetMainCamera();
		DXSimp::Vector3 camPos = cam.GetTransform().GetPosition();
		UpdateSubResource(2, &camPos);

		// �[�x�e�N�X�`��
		Texture& texture = InSceneSystemManager::GetInstance()->GetShadowDrawer().GetDepthRTV();
		SetTexture(2, &texture);
	}
}

