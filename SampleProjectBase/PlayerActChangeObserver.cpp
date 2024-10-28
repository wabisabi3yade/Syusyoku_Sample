#include "pch.h"
#include "PlayerActChangeObserver.h"
#include "PlayerActionController.h"
#include "PlayerActState_Base.h"

PlayerActChangeObserver::PlayerActChangeObserver(const std::string& _observerName, PlayerActionController& _actionController)
	: HashiTaku::IObserver<int>(_observerName), pActionController(&_actionController)
{
}

void PlayerActChangeObserver::Update(const int& _changeType)
{
	HASHI_DEBUG_LOG(GetObserverName() + "’Ê’mŽó‚¯Žæ‚è");

	pActionController->ChangeNode(static_cast<PlayerActState_Base::StateType>(_changeType));
}
