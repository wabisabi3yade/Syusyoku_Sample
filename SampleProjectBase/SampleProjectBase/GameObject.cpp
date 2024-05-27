#include "pch.h"
#include "GameObject.h"
#include "ModelCollect.h"

void GameObject::Release()
{
}

GameObject::GameObject() : model()
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

	
		model->Draw(transform);

}
