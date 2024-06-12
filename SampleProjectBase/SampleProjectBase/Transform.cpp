#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;
void Transform::ResetParameter()
{
	// ƒpƒ‰ƒ[ƒ^‰Šú‰»
	position = Vector3::Zero;
	rotation = Vector3::Zero;
	scale = Vector3::One;
	quaternion = Quaternion::Identity;
}
