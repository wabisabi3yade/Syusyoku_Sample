#include "pch.h"
#include "AppSystemDraw.h"
#include "AssetDisplay.h"
#include "DX11BulletPhisics.h"

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
		DX11BulletPhisics* pBulletEngine = DX11BulletPhisics::GetInstance();

		ImGui::Begin("System");

		pFrameRate->ImGuiCall();

		if (ImGuiMethod::TreeNode("Phisics"))
		{
			bool isDisplay = pBulletEngine->GetDisplay();
			if (ImGui::Checkbox("Display", &isDisplay))
				pBulletEngine->SetDisplay(isDisplay);

			float gravity = pBulletEngine->GetGravity();
			if (ImGui::DragFloat("Gravity", &gravity, 0.01f, -1000.0f, -9.81f));
			pBulletEngine->SetGravity(gravity);

			ImGui::TreePop();
		}

		AssetDisplay::Draw();

		ImGui::End();
	}
}