#include "pch.h"
#include "AppSystemDraw.h"
#include "AssetDisplay.h"
#include "DX11BulletPhisics.h"

AppSystemDraw::AppSystemDraw(VariableFrameRate& _frameRate, DX11BulletPhisics& _bulletEngine)
{
	pFrameRate = &_frameRate;
	pBulletEngine = &_bulletEngine;
}

void AppSystemDraw::ImGuiSetting()
{
	ImGui::Begin("System");

	AssetDisplay::Draw();
	pFrameRate->ImGuiCall();

	if (ImGuiMethod::TreeNode("Phisics"))
	{
		bool isDisplay = pBulletEngine->GetDisplay();
		ImGui::Checkbox("Display", &isDisplay);
		pBulletEngine->SetDisplay(isDisplay);

		ImGui::TreePop();
	}

	ImGui::End();
}
