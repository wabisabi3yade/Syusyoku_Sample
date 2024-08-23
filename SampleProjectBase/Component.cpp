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

bool Component::GetIsAlreadyStart() const
{
    return isAlreadyStart;
}

void Component::StartBase()
{
    Start();
    isAlreadyStart = true;
}

void Component::TransitionEnable()
{
    isEnable = !isEnable;

    if (isEnable)   // 活動状態になったらなら
        gameObject->AddActiveComponent(*this);
    else
        gameObject->RemoveActiveComponent(*this);
}

nlohmann::json Component::Save()
{
    nlohmann::json componentData;
    componentData["name"] = name;
    componentData["enable"] = isEnable;

    return componentData;
}

void Component::Load(const nlohmann::json& _componentData)
{
    bool loadEnable;
    HashiTaku::LoadJsonBoolean("enable", loadEnable, _componentData);
    SetEnable(loadEnable);

}

void Component::SetName(const std::string& _name)
{
    name = _name;
}

void Component::SetEnable(bool _enable)
{
    isEnable = _enable;

    if (isEnable)   // 活動状態になったらなら
        gameObject->AddActiveComponent(*this);
    else
        gameObject->RemoveActiveComponent(*this);
}

std::string Component::GetName() const
{
    return name;
}
