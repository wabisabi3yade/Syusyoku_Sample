#include "pch.h"
#include "GameObject.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "Component.h"
void GameObject::Update()
{
	if (!isActive) return;

	transform.UpdateVector();	// 方向ベクトルを更新する

	for (auto& itr : pComponents)
	{
		itr->Update();
	}
}

void GameObject::LateUpdate()
{
	if (!isActive) return;

	for (auto& itr : pComponents)
	{
		itr->LateUpdate();
	}
}

void GameObject::Draw()
{
	if (!isActive) return;

	for (auto& itr : pComponents)
	{
		itr->Draw();
	}
}

void GameObject::ImGuiSet()
{
#ifdef _DEBUG
	if (ImGui::TreeNode(name.c_str()))	// 名前Tree
	{
		ImGui::Checkbox("isActive", &isActive);
		ImGuiMethod::DragFloat3(transform.position, "pos");
		ImGuiMethod::DragFloat3(transform.rotation, "rot");
		ImGuiMethod::DragFloat3(transform.scale, "scale");

		for (auto& itr : pComponents)
		{
			itr->SetParameter();
		}

		ImGui::TreePop();
	}
#endif // _DEBUG
}
