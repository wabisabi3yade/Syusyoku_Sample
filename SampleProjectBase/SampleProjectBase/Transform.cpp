#include "pch.h"
#include "Transform.h"
using namespace DirectX::SimpleMath;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
void Transform::UpdateVector()
{
	// 回転行列をもとめる
	Matrix rotateMatrix = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(rotation.y),
		DirectX::XMConvertToRadians(rotation.x),
		DirectX::XMConvertToRadians(rotation.z)
	);

	// 更新する
	// rotateMatrix[0] → 右成分
	right.x = rotateMatrix.m[0][0];
	right.y = rotateMatrix.m[0][1];
	right.z = rotateMatrix.m[0][2];

	// rotateMatrix[1] → 上成分
	up.x = rotateMatrix.m[1][0];
	up.y = rotateMatrix.m[1][1];
	up.z = rotateMatrix.m[1][2];

	// rotateMatrix[2] → 前成分
	forward.x = rotateMatrix.m[2][0];
	forward.y = rotateMatrix.m[2][1];
	forward.z = rotateMatrix.m[2][2];
}


void Transform::ResetParameter()
{
	// パラメータ初期化
	position = Vector3::Zero;
	rotation = Vector3::Zero;
	scale = Vector3::One;
	quaternion = Quaternion::Identity;
}

void Transform::LookAt(DirectX::SimpleMath::Vector3 _worldPos, const DirectX::SimpleMath::Vector3 _upVector)
{
	// Atan2関数からベクトルで角度を求める
	Vector3 vec = _worldPos - position;
	vec.Normalize();
	forward = vec;	// 正面ベクトルとする
	up = Vector3::Up;	// 上方向
	right = up.Cross(forward);	// 右方向ベクトルを求める
	right.Normalize();
	up = forward.Cross(right);	// 真上方向を再計算する
	up.Normalize();	

	//float sy = sqrtf(forward.x * forward.x + up.x * up.x);
	//bool singular = sy < 1e-6;	// 近すぎるときに
	//if (!singular)
	//{
	//	rotation.x = atan2f(forward.y, forward.z) * Mathf::radToDeg;
	//	rotation.y = atan2f(-forward.x, sy) * Mathf::radToDeg;
	//	rotation.z = atan2f(up.x, right.x) * Mathf::radToDeg;
	//}
	//else {
	//	rotation.x = atan2f(-up.z, up.y) * Mathf::radToDeg;
	//	rotation.y = atan2f(-forward.x, sy) * Mathf::radToDeg;
	//	rotation.z = 0;
	//}

	Vector3 vec = _worldPos - position;
	float distance = vec.Length();
	if (distance == 0.0f)
	{
		position.z -= 0.0001f;
	}

	Matrix rotateMatrix = Matrix::CreateLookAt(position, _worldPos, Vector3::Up);
	// 更新する
	// rotateMatrix[0] → 右成分
	right.x = rotateMatrix.m[0][0];
	right.y = rotateMatrix.m[0][1];
	right.z = rotateMatrix.m[0][2];

	// rotateMatrix[1] → 上成分
	up.x = rotateMatrix.m[1][0];
	up.y = rotateMatrix.m[1][1];
	up.z = rotateMatrix.m[1][2];

	// rotateMatrix[2] → 前成分
	forward.x = rotateMatrix.m[2][0];
	forward.y = rotateMatrix.m[2][1];
	forward.z = rotateMatrix.m[2][2];
}
