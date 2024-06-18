#include "pch.h"
#include "BaseObject.h"

using namespace DirectX::SimpleMath;

Transform BaseObject::transform = {};
std::unique_ptr<Cube> BaseObject::pCube = nullptr;
Color BaseObject::color = Color(1.0f, 1.0f, 1.0f, 1.0f);

void BaseObject::Init()
{
	pCube = std::make_unique<Cube>();
}

void BaseObject::DrawCube()
{
	pCube->Draw(transform, color);
}
