#pragma once
#include "IObserever.h"

class PlayerActionController;

/// @brief プレイヤーのアクション状態を遷移するときに通知されるクラス
class PlayerActChangeObserver : public HashiTaku::IObserver<int>
{
	PlayerActionController* pActionController;
public:
	/// @brief コンストラクタ
	/// @param _observerName オブザーバー名
	/// @param _actionController アクションコントローラー
	PlayerActChangeObserver(const std::string& _observerName, PlayerActionController& _actionController);
	~PlayerActChangeObserver() {}

	void Update(const int& _changeType);
};

