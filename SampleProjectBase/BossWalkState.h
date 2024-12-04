#pragma once
#include "BossGroundMove.h"

/// @brief ƒ{ƒX•à‚«
class BossWalkState : public BossGroundMove
{
public: 
	BossWalkState();
	~BossWalkState() {}

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void TransitionCheckUpdate() override;

	void ImGuiDebug() override;
};

