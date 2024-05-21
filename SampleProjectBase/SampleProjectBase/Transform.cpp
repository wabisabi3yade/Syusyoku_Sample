#include "pch.h"
#include "Transform.h"

Transform::Transform() : position(Float3::zero), rotation(Float3::zero), scale(Float3::one)
						,Quaternion(Float4::zero)
{
}

Transform::~Transform()
{
}
