#include "pch.h"

#include "D3D11_Renderer.h"
#include "RenderParam.h"

using namespace DirectX;
using namespace SimpleMath;

const RenderParam::WVP& RenderParam::GetWVP(const Transform& _transform)
{
	return GetWVP(_transform.GetPosition(), _transform.GetScale(), _transform.GetRotation());
}

const RenderParam::WVP& RenderParam::GetWVP(const Vector3& _position, const Vector3& _scale, const Quaternion& _rotation)
{
	// 変換行列を作成
	// 移動行列
	Matrix t = XMMatrixTranslation(
		_position.x,
		_position.y,
		_position.z
	);

	// スケーリング行列
	Matrix s = XMMatrixScaling(
		_scale.x,
		_scale.y,
		_scale.z
	);

	// 回転行列
	Matrix r = Matrix::CreateFromQuaternion(_rotation);

	Matrix worldMtx = s * r * t;	// ワールド変換行列を作成

	worldMtx = worldMtx.Transpose();

	wvp.world = worldMtx;

	return wvp;
}

