#include "pch.h"
#include "AppSystemDraw.h"
#include "AssetDisplay.h"
#include "DX11BulletPhisics.h"

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
		ImGui::Checkbox("Display", &isDisplay);
		pBulletEngine->SetDisplay(isDisplay);

		ImGui::TreePop();
	}

	AssetDisplay::Draw();

	ImGui::End();
}
