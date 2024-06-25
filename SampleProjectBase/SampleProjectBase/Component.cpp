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
