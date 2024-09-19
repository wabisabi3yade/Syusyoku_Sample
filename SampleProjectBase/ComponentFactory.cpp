#include "pch.h"
#include "ComponentFactory.h"

#include "GameObject.h"

void ComponentFactory::Init()
{
	// 当たり判定
	ResistComponnent<CP_RigidBody>();
	ResistComponnent<CP_BoxCollider>();
	ResistComponnent<CP_SphereCollider>();

	// 描画
	ResistComponnent<CP_MeshRenderer>();
	ResistComponnent<CP_SpriteRenderer>();
	ResistComponnent<CP_SkyBox>();

	// アニメーション
	ResistComponnent<CP_Animation>();
	ResistComponnent<CP_Weapon>();

	// カメラ
	ResistComponnent<CP_Camera>();
	ResistComponnent<CP_CameraMove>();

	// プレイヤー
	ResistComponnent<CP_Player>();

	// その他
	ResistComponnent<CP_EaseTest>();
	ResistComponnent<CP_BoxAnimation>();
	ResistComponnent<CP_NotifyTest>();
}

std::unique_ptr<Component> ComponentFactory::CreateByName(const std::string& _componentName)
{
	auto itr = pComponents.find(_componentName);

	if (itr == pComponents.end())
	{
		HASHI_DEBUG_LOG(_componentName + "はリストにありません");
		return nullptr;
	}

	return itr->second->Clone();
}

void ComponentFactory::CreateImGuiCombo(GameObject& _targetObject)
{
#ifdef EDIT
	static std::string compName = "";
	std::vector<const std::string*> conponentsName = GetComponentsName();

	if (ImGui::Button("Add"))
	{
		_targetObject.SetComponent(CreateByName(compName));
	}

	ImGuiMethod::ComboBox("Component", compName, conponentsName);
#endif // EDIT
}

std::vector<const std::string*> ComponentFactory::GetComponentsName()
{
	std::vector<const std::string*> s;

	for (auto& itr : pComponents)
	{
		s.push_back(&itr.first);
	}

	return s;
}
