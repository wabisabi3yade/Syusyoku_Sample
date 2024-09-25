#pragma once
#include "CP_Animation.h"

// プレイヤーのアニメーション
class CP_PlayerAnimation : public CP_Animation
{

public:
	CP_PlayerAnimation() {}
	~CP_PlayerAnimation() {}

	void Init() override;
};

