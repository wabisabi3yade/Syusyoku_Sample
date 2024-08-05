#pragma once
#include "Component.h"

/// @brief イージングテスト用
class CP_EaseTest : public Component
{
	DirectX::SimpleMath::Color color;

	HashiTaku::EaseFunction easeFunc;
	HashiTaku::EaseType nowEase;

	float magnitude{15.0f};
	float time = 1.0f;
	float elapsedTime = 0.0f;
	short int vec = 1;
	float p = 0;
public:
	CP_EaseTest();
	~CP_EaseTest() {}

	void Init()override;

	void Update()override;

	void Draw()override;

	void ImGuiSetting()override;
};

