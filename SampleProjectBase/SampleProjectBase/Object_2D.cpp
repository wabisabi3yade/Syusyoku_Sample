#include "pch.h"
#include "Object_2D.h"

#include "CP_SpriteRenderer.h"

using namespace DirectX::SimpleMath;

Object_2D::Object_2D() 
{
	AddComponent<CP_SpriteRenderer>();
}
