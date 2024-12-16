#pragma once
#include "CharacterActionController.h"
#include "ChangeAnimObserver.h"

namespace HashiTaku
{
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

	protected:
		/// @brief 各Stateの文字列を取得する
		/// @param _stateId 状態のID
		/// @return 文字列
		virtual std::string GetStateStr(int _stateId) = 0;

		/// @brief 各StateのIDを取得する
		/// @param _stateName　状態名
		/// @return 状態のID
		virtual int GetStateId(const std::string& _stateName) = 0;
	};
}