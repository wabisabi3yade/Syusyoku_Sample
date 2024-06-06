#include "pch.h"
#include "GameObject.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;
void GameObject::Release()
{
}

GameObject::GameObject() : pConstModel()
{
}

GameObject::~GameObject()
{
	Release();
}

void GameObject::Update()
{
}

void GameObject::Draw()
{
	pConstModel->Draw(transform);
}
