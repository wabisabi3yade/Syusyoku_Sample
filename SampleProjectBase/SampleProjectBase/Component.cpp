#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Transform& Component::GetTransform()
{
    return gameObject->transform;
}
