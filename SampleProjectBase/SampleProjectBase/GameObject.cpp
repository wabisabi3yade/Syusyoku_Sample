#include "pch.h"
#include "GameObject.h"

void GameObject::Release()
{
}

GameObject::GameObject() : model(nullptr)
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
	if(model->GetImported())	// モデルにデータが入っているなら
	model->Draw(transform);
}
