#pragma once
#include "CloneComponent.h"
#include "AnimationNotify_Base.h"

class CP_NotifyTest : public Component, public CloneComponent<CP_NotifyTest>
{
	std::list<std::unique_ptr<AnimationNotify_Base>> pNotifys;

	float lastRatio{ -Mathf::smallValue };
	float curRatio{ 0.0f };
	float speed{ 1.0f };
public:
	CP_NotifyTest() {}
	CP_NotifyTest(const CP_NotifyTest& _other);
	~CP_NotifyTest() {}
	CP_NotifyTest& operator=(const CP_NotifyTest& _other);

	void Update();
	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void Copy(const CP_NotifyTest& _other);
};

