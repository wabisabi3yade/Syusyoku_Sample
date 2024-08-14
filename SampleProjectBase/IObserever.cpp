#include "pch.h"
#include "IObserever.h"

void HashiTaku::Subject::AddObserver(IObserver& _observer)
{
	auto itr = std::find(observers.begin(), observers.end(), &_observer);

	if (itr != observers.end()) return;

	observers.push_back(&_observer);
}

void HashiTaku::Subject::RemoveObserver(IObserver& _observer)
{
	observers.remove(&_observer);
}

void HashiTaku::Subject::NotifyAll()
{
	for (auto& o : observers)
	{
		o->Update();
	}
}
