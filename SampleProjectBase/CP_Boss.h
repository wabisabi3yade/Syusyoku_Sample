#pragma once
#include "CP_Enemy.h"
#include "BossActionController.h"

/// @brief �{�X�̃R���|�[�l���g
class CP_Boss : public CP_Enemy
{
	std::unique_ptr<BossActionController> pActionController;

	/// @brief �A�j���[�V����
	CP_Animation* pAnimation;

	
public:
	CP_Boss();
	~CP_Boss() {}

	void Init() override;

private:
};

