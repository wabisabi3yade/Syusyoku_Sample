#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Transform& Component::GetTransform() const
{
    return gameObject->transform;
}
