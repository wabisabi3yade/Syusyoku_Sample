#pragma once
#include "CloneComponent.h"

/// @brief イージングテスト用
class CP_EaseTest :public Component, public CloneComponent<CP_EaseTest>
{
	DirectX::SimpleMath::Color color;

	HashiTaku::EaseKind nowEase;

	float magnitude{ 15.0f };
	float time = 1.0f;
	float elapsedTime = 0.0f;
	short int vec = 1;
	float p = 0;
public:
	CP_EaseTest();
	~CP_EaseTest() {}

	void Start()override;

	void Update()override;

	void Draw()override;

	void ImGuiSetting()override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
};

