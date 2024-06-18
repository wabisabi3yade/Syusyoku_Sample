#include "pch.h"
#include "Object_3D.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;
void Object_3D::Release()
{
}

Object_3D::Object_3D() : pModel(nullptr)
{
}

Object_3D::~Object_3D()
{
	Release();
}

void Object_3D::Update()
{
	GameObject::Update();
}

void Object_3D::LateUpdate()
{
	GameObject::LateUpdate();
}

void Object_3D::Draw()
{
	GameObject::Draw();

	/*if(pModel)
	pModel->Draw(transform);*/
}
