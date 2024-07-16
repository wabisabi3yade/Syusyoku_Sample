#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Transform& Component::GetTransform() const
{
    return gameObject->transform;
}

Component::Component(GameObject* _gameObject) : gameObject(_gameObject), isEnable(true)
{
}

Component& Component::operator=(const Component& _other)
{
    if (this == &_other) return *this;  // “¯‚¶‚È‚ç•Ô‚·
    
    isEnable = _other.isEnable;

    return *this;
}
