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
	rotation.x = DirectX::XMConvertToDegrees(atan2(vec.y, vec.z));
	rotation.y = DirectX::XMConvertToDegrees(atan2(vec.x, vec.z));
	rotation.z = DirectX::XMConvertToDegrees(atan2(vec.x, vec.y));

	UpdateVector();	// 方向ベクトルを更新

	// 逆さまにならないように調整
	// こいつの上向きベクトルが指定の上ベクトルと真逆なら
	float dotUp = up.Dot(_upVector);	
	if (dotUp < 0.0f)
	{
		ImGuiDebugLog::Add("逆さま");
	}
}
