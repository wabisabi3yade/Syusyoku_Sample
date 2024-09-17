#pragma once
#include "Component.h"
#include "AnimationNotify_Base.h"

class CP_NotifyTest : public Component
{
	std::list<std::unique_ptr<AnimationNotify_Base>> pNotifys;

	float lastRatio{ 0.0f };
	float curRatio{ 0.0f };
	float speed{ 1.0f };
public:
	CP_NotifyTest() {}
	~CP_NotifyTest() {}

	void Update();

	void ImGuiSetting() override;
};

