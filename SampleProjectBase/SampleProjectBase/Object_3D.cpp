#include "pch.h"
#include "Object_3D.h"

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
	// ���f���̃p�����[�^��ImGui�ōX�V
	pModel->ImGuiSetting();
}

void Object_3D::Draw()
{
	if (!isActive) return;

	GameObject::Draw();

	if(pModel)
	pModel->Draw(transform);
}
