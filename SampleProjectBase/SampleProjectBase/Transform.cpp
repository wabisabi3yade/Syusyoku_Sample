#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;
void Transform::ResetParameter()
{
	// パラメータ初期化
	position = Vector3::Zero;
	rotation = Vector3::Zero;
	scale = Vector3::One;
	quaternion = Quaternion::Identity;
}
