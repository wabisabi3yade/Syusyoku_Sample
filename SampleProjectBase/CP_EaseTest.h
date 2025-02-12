#pragma once
#include "Component.h"

namespace HashiTaku
{
	/// @brief イージングテスト用
	class CP_EaseTest : public Component
	{
		DXSimp::Color color;

		EaseKind nowEase;

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

		void ImGuiDebug()override;

		json Save() override;
		void Load(const json& _data) override;
	};
}