#pragma once
#include "Component.h"

/// @brief �A�j���[�V�����̒ʒm�C�x���g�R���|�[�l���g
class CP_AnimationNotify : public Component
{
public:
	CP_AnimationNotify();
	~CP_AnimationNotify() {}

private:
	void Start() override;
	void LateUpdate() override;
};

