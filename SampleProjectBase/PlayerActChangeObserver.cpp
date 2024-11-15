#include "pch.h"
#include "PlayerActChangeObserver.h"
#include "PlayerActionController.h"
#include "PlayerActState_Base.h"

PlayerActChangeObserver::PlayerActChangeObserver(const std::string& _observerName, PlayerActionController& _actionController)
	: HashiTaku::IObserver<int>(_observerName), pActionController(&_actionController)
{
}

void PlayerActChangeObserver::ObserverUpdate(const int& _changeType)
{
	HASHI_DEBUG_LOG(GetObserverName() + "通知受け取り");

	pActionController->ChangeState(static_cast<PlayerActState_Base::PlayerState>(_changeType));
}
