#pragma once
#include "BossActState_Base.h"

/// @brief �u���C�N���̍s��
class BossBreakIdle : public BossActState_Base
{

public:
	BossBreakIdle() {}
	~BossBreakIdle() {}

	/// @brief �J�ڍX�V
	void TransitionCheckUpdate() override;
};

