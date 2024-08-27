#include "pch.h"
#include "ComponentFactory.h"

#include "GameObject.h"

void ComponentFactory::Init()
{
	// �����蔻��
	ResistComponnent<CP_RigidBody>();
	ResistComponnent<CP_BoxCollider>();
	ResistComponnent<CP_SphereCollider>();

	// �`��
	ResistComponnent<CP_MeshRenderer>();
	ResistComponnent<CP_SpriteRenderer>();

	// �A�j���[�V����
	ResistComponnent<CP_Animation>();
	ResistComponnent<CP_Weapon>();

	// �J����
	ResistComponnent<CP_Camera>();
	ResistComponnent<CP_CameraMove>();

	// �v���C���[
	ResistComponnent<CP_Player>();

	// ���̑�
	ResistComponnent<CP_EaseTest>();
	ResistComponnent<CP_BoxAnimation>();
}

std::unique_ptr<Component> ComponentFactory::CreateByName(const std::string& _componentName)
{
	auto itr = pComponents.find(_componentName);

	if (itr == pComponents.end())
	{
		HASHI_DEBUG_LOG(_componentName + "�̓��X�g�ɂ���܂���");
		return nullptr;
	}

	return itr->second->Clone();
}

void ComponentFactory::CreateImGuiCombo(GameObject& _targetObject)
{
#ifdef EDIT
	static std::string compName = "";
	static std::vector<std::string> conponentsName = GetComponentsName();

	if (ImGui::Button("Add"))
	{
		_targetObject.SetComponent(CreateByName(compName));
	}

	ImGuiMethod::ComboBox("Component", compName, conponentsName);
#endif // EDIT
}

std::vector<std::string> ComponentFactory::GetComponentsName()
{
	std::vector<std::string> s;

	for (auto& itr : pComponents)
	{
		s.push_back(itr.first);
	}

	return s;
}
