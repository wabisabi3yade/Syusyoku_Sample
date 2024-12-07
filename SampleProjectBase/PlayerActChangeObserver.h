#pragma once
#include "IObserever.h"

class PlayerGroundActionController;

/// @brief プレイヤーのアクション状態を遷移するときに通知されるクラス
class PlayerActChangeObserver : public HashiTaku::IObserver<int>
{
	PlayerGroundActionController* pActionController;
public:
	/// @brief コンストラクタ
	/// @param _observerName オブザーバー名
	/// @param _actionController アクションコントローラー
	PlayerActChangeObserver(const std::string& _observerName, PlayerGroundActionController& _actionController);
	~PlayerActChangeObserver() {}

	void ObserverUpdate(const int& _changeType);
};

