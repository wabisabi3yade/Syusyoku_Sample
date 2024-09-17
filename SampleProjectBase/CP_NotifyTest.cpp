#include "pch.h"
#include "CP_NotifyTest.h"

#include "AnimationNotifyFactory.h"

void CP_NotifyTest::Update()
{
	curRatio += speed * MainApplication::DeltaTime();
	bool isLoop = false;

	if (curRatio > 1.0f)
	{
		curRatio -= 1.0f;
		isLoop = true;
	}
	
	for (auto& n : pNotifys)
	{
		n->Update(lastRatio, curRatio, isLoop);
	}

	lastRatio = curRatio;
}

void CP_NotifyTest::ImGuiSetting()
{
	ImGui::DragFloat("speed", &speed, 1.0f, 0.0f, 100.0f);
	ImGui::SliderFloat("curRatio", &curRatio, 0.0f, 1.0f);
	ImGui::SliderFloat("lastRatio", &lastRatio, 0.0f, 1.0f);
}
