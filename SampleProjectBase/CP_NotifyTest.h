#pragma once
#include "Component.h"
#include "AnimationNotify_Base.h"
#include "AnimationNotifyFactory.h"

class CP_NotifyTest : public Component
{
	std::list<std::unique_ptr<AnimationNotify_Base>> pNotifys;

	std::unique_ptr<AnimationNotifyFactory> pFactory;

	float lastRatio{ -Mathf::smallValue };
	float curRatio{ 0.0f };
	float speed{ 1.0f };
public:
	CP_NotifyTest() {}
	CP_NotifyTest(const CP_NotifyTest& _other);
	~CP_NotifyTest() {}
	CP_NotifyTest& operator=(const CP_NotifyTest& _other);

	void Init() override;
	void Update();
	void ImGuiDebug() override;

	json Save() override;
	void Load(const json& _data) override;
private:
	void Copy(const CP_NotifyTest& _other);
};

