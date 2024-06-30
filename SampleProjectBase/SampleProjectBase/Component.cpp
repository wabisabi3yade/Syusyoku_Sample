#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Transform& Component::GetTransform()
{
    return gameObject->transform;
}

Component::Component(GameObject* _gameObject) : gameObject(_gameObject), isStartYet(false),
    isEnable(true)
{
}

Component& Component::operator=(const Component& _other)
{
    if (this == &_other) return *this;  // “¯‚¶‚È‚ç•Ô‚·
    
    isStartYet = _other.isStartYet;
    isEnable = _other.isEnable;

    return *this;
}
