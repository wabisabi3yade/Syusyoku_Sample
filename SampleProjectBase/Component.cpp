#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Transform& Component::GetTransform() const
{
    return gameObject->transform;
}

void Component::Copy(const Component& _other)
{
    if (this == &_other) return;

    isEnable = _other.isEnable;
    name = _other.name;
    gameObject = _other.gameObject;
}

Component& Component::operator=(const Component& _other)
{
    Copy(_other);

    return *this;
}
