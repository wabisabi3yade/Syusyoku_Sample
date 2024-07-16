#include "pch.h"

#include "D3D11_Renderer.h"
#include "RenderParam.h"

using namespace DirectX;
using namespace SimpleMath;

ID3D11InputLayout& RenderParam::GetInputLayout()
{
	return *pInputLayout.Get();
}

const RenderParam::WVP& RenderParam::GetWVP(const Transform& _transform)
{
	// 変換行列を作成
	// 移動行列
	Matrix t = XMMatrixTranslation(
		_transform.position.x,
		_transform.position.y,
		_transform.position.z
	);

	// スケーリング行列
	Matrix s = XMMatrixScaling(
		_transform.scale.x,
		_transform.scale.y,
		_transform.scale.z
	);

	// 回転行列
	Matrix r = Matrix::CreateFromYawPitchRoll(
		_transform.rotation.y * Mathf::degToRad,
		_transform.rotation.x * Mathf::degToRad,
		_transform.rotation.z * Mathf::degToRad
	);

	Matrix worldMtx = s * r * t;	// ワールド変換行列を作成

	worldMtx = worldMtx.Transpose();

	wvp.world = worldMtx;

	return wvp;
}

