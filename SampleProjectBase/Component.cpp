#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Transform& Component::GetTransform() const
{
    return gameObject->transform;
}

bool Component::GetIsEnable() const
{
    return isEnable;
}

void Component::TransitionEnable()
{
    isEnable = !isEnable;

    if (isEnable)   // 活動状態になったらなら
        gameObject->AddActiveComponent(*this);
    else
        gameObject->RemoveActiveComponent(*this);
}

std::string Component::GetName() const
{
    return name;
}
