#pragma once
#include "CP_Enemy.h"
#include "BossActionController.h"

/// @brief ボスのコンポーネント
class CP_Boss : public CP_Enemy
{
	std::unique_ptr<BossActionController> pActionController;

	/// @brief アニメーション
	CP_Animation* pAnimation;

	
public:
	CP_Boss();
	~CP_Boss() {}

	void Init() override;

private:
};

