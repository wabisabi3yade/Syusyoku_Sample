#include "pch.h"
#include "Transform.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
void Transform::UpdateVector()
{
	// 回転行列を求める
	Matrix rotateMatrix = Matrix::CreateFromYawPitchRoll(
		rotation.y * Mathf::degToRad,
		rotation.x * Mathf::degToRad,
		rotation.z * Mathf::degToRad
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
	// 距離がないなら
	Vector3 vec = _worldPos - position;
	float distance = vec.Length();
	if (distance == 0.0f)
		position.z -= 0.0001f;	// 少しずらす
	vec.Normalize();

	// 座標へのビュー行列を求め、逆行列にする
	Matrix lookMatrix = DirectX::XMMatrixLookAtLH(position, _worldPos, Vector3::Up);
	Matrix invertMatrix = lookMatrix.Invert();

	// 更新する
	// invertMatrix[0] → 右成分
	right.x = invertMatrix.m[0][0];
	right.y = invertMatrix.m[0][1];
	right.z = invertMatrix.m[0][2];

	// invertMatrix[1] → 上成分
	up.x = invertMatrix.m[1][0];
	up.y = invertMatrix.m[1][1];
	up.z = invertMatrix.m[1][2];

	// invertMatrix[2] → 前成分
	forward.x = invertMatrix.m[2][0];
	forward.y = invertMatrix.m[2][1];
	forward.z = invertMatrix.m[2][2];

	// Atan2関数からベクトルで角度を求める
	// 正面ベクトルとする
	forward = vec;
	up = Vec3::Up;

	// 右方向ベクトルを求める
	right = forward.Cross(up);
	right.Normalize();

	// 真上方向を再計算する
	up = right.Cross(forward);
	up.Normalize();

	// 各軸角度を求める
	// Y軸
	Vector3 v = up;

	// 求めたい軸の成分を0(XZ平面)
	v.y = 0.0f;
	v.Normalize();

	// 基準のワールド系の単位軸ベクトルと内積
	float dotXZ = Vec3::Dot(v, Vec3::Right);
	rotation.y = acosf(dotXZ) * Mathf::radToDeg - 90.0f;

	// X軸
	v = right;
	v.x = 0.0f;
	v.Normalize();
	float dotYZ = Vec3::Dot(v, Vec3::Forward);
	rotation.x = acosf(dotYZ) * Mathf::radToDeg;

	// Z軸
	v = forward;
	v.z = 0.0f;
	v.Normalize();
	float dotXY = Vec3::Dot(v, Vec3::Right);
	rotation.z = acosf(dotXY) * Mathf::radToDeg;
}
