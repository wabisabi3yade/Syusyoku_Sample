#include "pch.h"
#include "AppSystemDraw.h"
#include "AssetDisplay.h"
#include "DX11BulletPhisics.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	void AppSystemDraw::Init(VariableFrameRate& _variabeFrameRate)
	{
		pFrameRate = &_variabeFrameRate;
	}

	AppSystemDraw::AppSystemDraw() : pFrameRate(nullptr)
	{

	}

	void AppSystemDraw::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Begin("System");

		// �^�C���X�P�[��
		ImGuiSceneTimeScale();

		// �t���[�����[�g�\��
		pFrameRate->ImGuiCall();

		// �����G���W��
		ImGuiPhisics();

		// �A�Z�b�g
		AssetDisplay::Draw();

		// �����_�[�^�[�Q�b�g
		ImGuiRenderTarget();

		ImGui::End();
#endif // EDIT
	}

	void AppSystemDraw::ImGuiSceneTimeScale()
	{
#ifdef EDIT
		auto* pInScene = InSceneSystemManager::GetInstance();
		float timeScale = pInScene->GetTimeScale();

		if (ImGui::DragFloat("SceneTimeScale", &timeScale, 0.01f, 0.0f, 100.0f))
		{
			pInScene->SetTimeScale(timeScale);
		}
#endif // EDIT
	}

	void AppSystemDraw::ImGuiPhisics()
	{
#ifdef EDIT
		if (ImGuiMethod::TreeNode("Phisics"))
		{
			DX11BulletPhisics* pBulletEngine = DX11BulletPhisics::GetInstance();

			// �����蔻��\��
			bool isDisplay = pBulletEngine->GetDisplay();
			if (ImGui::Checkbox("Display", &isDisplay))
				pBulletEngine->SetDisplay(isDisplay);

			// �d��
			float gravity = pBulletEngine->GetGravity();
			if (ImGui::DragFloat("Gravity", &gravity, 0.01f, -1000.0f, -9.81f))
				pBulletEngine->SetGravity(gravity);

			ImGui::TreePop();
		}
#endif // EDIT
	}

	void AppSystemDraw::ImGuiRenderTarget()
	{
#ifdef EDIT
		if (!ImGuiMethod::TreeNode("RenderTarget")) return;

		auto& pShadowDrawer = InSceneSystemManager::GetInstance()->GetShadowDrawer();
		pShadowDrawer.ImGuiCall();

		ImGui::TreePop();
#endif // EDIT
	}
}