#include "pch.h"
#include "GameObject.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "Component.h"
void GameObject::Update()
{
	for (auto& itr : components)
	{
		itr->Update();
	}
}

void GameObject::LateUpdate()
{
	for (auto& itr : components)
	{
		itr->LateUpdate();
	}
}

void GameObject::Draw()
{
	for (auto& itr : components)
	{
		itr->Draw();
	}
}

void GameObject::ImGuiSet()
{
#ifdef _DEBUG
	if (ImGui::TreeNode(name.c_str()))	// –¼‘OTree
	{
		ImGuiMethod::DragFloat3(transform.position, "pos");
		ImGuiMethod::DragFloat3(transform.rotation, "rot");
		ImGuiMethod::DragFloat3(transform.scale, "scale");

		ImGui::TreePop();
	}
#endif // _DEBUG
}
