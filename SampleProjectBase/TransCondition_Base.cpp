#include "pch.h"
#include "TransCondition_Base.h"

TransCondition_Base::TransCondition_Base(const std::string& _parameterName, HashiTaku::AnimParam::TypeKind _typeKind)
	: pParameterName(&_parameterName), typeKind(_typeKind)
{
}

const std::string& TransCondition_Base::GetReferenceParamName() const
{
	return *pParameterName;
}

HashiTaku::AnimParam::TypeKind TransCondition_Base::GetTypeKind() const
{
	return typeKind;
}

void TransCondition_Base::ImGuiSetting()
{
	ImGui::Text(pParameterName->c_str());
}
