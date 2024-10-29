#include "pch.h"
#include "AnimParamRemoveObserver.h"

#include "AnimTransitionArrow.h"

HashiTaku::AnimParam::AnimParamObserver::AnimParamObserver(IAnimParamRelater& _arrow, const std::string& _observerName)
	: IObserver(_observerName), pInterface(&_arrow)
{
}

void HashiTaku::AnimParam::AnimParamObserver::ObserverUpdate(const NotificationData& _notifyData)
{
	pInterface->AcceptAnimParamData(_notifyData);
	HASHI_DEBUG_LOG(GetObserverName() + "‚ª’Ê’m‚³‚ê‚Ü‚µ‚½");
}