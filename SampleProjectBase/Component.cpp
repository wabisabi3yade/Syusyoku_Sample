#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Transform& Component::GetTransform() const
{
    return gameObject->GetTransform();
}

bool Component::GetIsEnable() const
{
    return isEnable;
}

bool Component::GetIsAlreadyAwake() const
{
    return isAlreadyAwake;
}

bool Component::GetIsAlreadyStart() const
{
    return isAlreadyStart;
}

void Component::AwakeCall()
{
    Awake();
    isAlreadyAwake = true;
}

void Component::StartCall()
{
    Start();
    isAlreadyStart = true;
}

void Component::UpdateCall()
{
    Update();
}

void Component::LateUpdateCall()
{
    LateUpdate();
}

void Component::DrawCall()
{
    Draw();
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

void Component::OnEnableTrueCall()
{
    OnEnableTrue();
    gameObject->AddActiveComponent(*this);
}

void Component::OnEnableFalseCall()
{
    OnEnableFalse();
    gameObject->RemoveActiveComponent(*this);
}

void Component::ImGuiSettingCall()
{
#ifdef EDIT

    if (ImGui::Checkbox("isEnable", &isEnable))
    {
        if (isEnable)
            OnEnableTrueCall();
        else
            OnEnableFalseCall();
    }

    ImGuiCall();

#endif // EDIT
}

bool Component::GetIsActive()
{
    return gameObject->GetIsActive() && isEnable;
}

void Component::SetName(const std::string& _name)
{
    name = _name;
}

void Component::SetEnable(bool _enable)
{
    if (isEnable == _enable) return;

    isEnable = _enable;

    if (isEnable)   // 活動状態になったらなら
        OnEnableTrueCall();
    else
        OnEnableFalseCall();
}

std::string Component::GetName() const
{
    return name;
}
