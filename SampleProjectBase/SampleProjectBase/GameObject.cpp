#include "pch.h"
#include "GameObject.h"
#include "ModelCollect.h"

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
	ModelCollect* m = ModelCollect::GetInstance();

	
		pConstModel->Draw(transform);

}
