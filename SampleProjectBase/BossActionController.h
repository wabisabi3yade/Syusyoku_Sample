#pragma once
#include "EnemyActionController.h"
#include "BossActState_Base.h"

/// @brief ボスのアクションコントローラー
class BossActionController : public EnemyActionController
{


public:
	BossActionController(CP_Character& _boss);
	~BossActionController() {}

	/// @brief 初期化処理
	/// @param _animationController 
	void Init(CP_Animation& _animationController) override;

	/// @brief 更新処理
	void Update() override;

	/// @brief ステートを変更
	/// @param _nextState 次のステート
	/// @return 成功したか？
	bool ChangeState(BossActState_Base::BossState _nextState);
};

