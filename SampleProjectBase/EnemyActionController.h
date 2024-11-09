#pragma once
#include "CharacterActionController.h"
#include "ChangeAnimObserver.h"

class CP_Enemy;
class CP_Animation;

/// @brief 敵のアクション管理クラス
/// @tparam T アクションのステートを表す列挙型
class EnemyActionController : public CharacterActionController
{

public:
	/// @brief コンストラクタ
	/// @param _enemy エネミーコンポーネント
	/// @param _controllerName アクションコントローラー名
	EnemyActionController(CP_Enemy& _enemy, const std::string& _controllerName);
	virtual ~EnemyActionController() {}
};
