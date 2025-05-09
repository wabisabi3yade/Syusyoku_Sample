#include "pch.h"
#include "AppSystemDraw.h"
#include "AssetDisplay.h"
#include "DX11BulletPhisics.h"
#include "RenderTargetCollection.h"
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

		// タイムスケール
		ImGuiSceneTimeScale();

		// フレームレート表示
		pFrameRate->ImGuiCall();

		// 物理エンジン
		ImGuiPhisics();

		// Effekseer
		ImGuiEffekseer();

		// アセット
		AssetDisplay::Draw();

		// レンダーターゲット
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

			// 当たり判定表示
			bool isDisplay = pBulletEngine->GetDisplay();
			if (ImGui::Checkbox("Display", &isDisplay))
				pBulletEngine->SetDisplay(isDisplay);

			// 重力
			float gravity = pBulletEngine->GetGravity();
			if (ImGui::DragFloat("Gravity", &gravity, 0.01f, -1000.0f, -9.81f))
				pBulletEngine->SetGravity(gravity);

			ImGui::TreePop();
		}
#endif // EDIT
	}

	void AppSystemDraw::ImGuiEffekseer()
	{
#ifdef EDIT
		if (!ImGuiMethod::TreeNode("VFX")) return;

		bool* isDisplay = &DX11EffekseerManager::GetInstance()->isVfxDisplay;
		ImGui::Checkbox("Display VFX", isDisplay);

		ImGui::TreePop();
#endif // EDIT

	}

	void AppSystemDraw::ImGuiRenderTarget()
	{
#ifdef EDIT
		if (!ImGuiMethod::TreeNode("RenderTarget")) return;

		// 影
		if (ImGuiMethod::TreeNode("Shadow"))
		{
			auto& pShadowDrawer = InSceneSystemManager::GetInstance()->GetShadowDrawer();
			pShadowDrawer.ImGuiCall();
			ImGui::TreePop();
		}

		// レンダーターゲットを取得して描画
		u_int rtvNum = static_cast<u_int>(RenderTargetCollection::RenderTargetType::MaxNum);
		for (u_int r_i = 0; r_i < rtvNum; r_i++)
		{
			RenderTargetCollection* pRTCol = &Direct3D11::GetInstance()->GetRenderer()->GetRTCollection();

			// レンダーターゲットをを取得する
			RenderTargetCollection::RenderTargetType rtvType = 
				static_cast<RenderTargetCollection::RenderTargetType>(r_i);
			RenderTarget* pRtv = pRTCol->GetRenderTarget(rtvType);
			if (!pRtv) continue;

			// 描画
			std::string typeStr = std::string(magic_enum::enum_name(rtvType));
			if (ImGuiMethod::TreeNode(typeStr))
			{
				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				ImGui::Image((void*)&pRtv->GetSRV(), viewportSize);
				
				ImGui::TreePop();
			}
		}
		
		ImGui::TreePop();
#endif // EDIT
	}
}