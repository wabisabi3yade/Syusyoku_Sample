#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;


void Transform::UpdateVector()
{
	//前向きリセット
	Vector3 initForward = Vector3::Forward;

	// 回転行列をもとめる
	Matrix rotateMatrix = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(rotation.y),
		DirectX::XMConvertToRadians(rotation.x),
		DirectX::XMConvertToRadians(rotation.z)
	);
	rotateMatrix.Transpose();

	forward = Vector3::Transform(initForward, rotateMatrix);	// 更新する
	forward.Normalize();	// 正規化
}


void Transform::ResetParameter()
{
	// パラメータ初期化
	position = Vector3::Zero;
	rotation = Vector3::Zero;
	scale = Vector3::One;
	quaternion = Quaternion::Identity;
}

void Transform::LookAt(DirectX::SimpleMath::Vector3 _worldPos)
{

}
