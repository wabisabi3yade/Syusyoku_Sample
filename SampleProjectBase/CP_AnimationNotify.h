#pragma once
#include "Component.h"

/// @brief アニメーションの通知イベントコンポーネント
class CP_AnimationNotify : public Component
{
public:
	CP_AnimationNotify();
	~CP_AnimationNotify() {}

private:
	void Start() override;
	void LateUpdate() override;
};

