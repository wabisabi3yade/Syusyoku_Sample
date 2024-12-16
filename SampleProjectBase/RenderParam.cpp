#include "pch.h"

#include "D3D11_Renderer.h"
#include "RenderParam.h"

namespace HashiTaku
{
	RenderParam::WVP& RenderParam::GetWVP()
	{
		return wvp;
	}

	RenderParam::WVP& RenderParam::GetWVP(const Transform& _transform)
	{
		return GetWVP(_transform.GetPosition(), _transform.GetScale(), _transform.GetRotation());
	}

	RenderParam::WVP& RenderParam::GetWVP(const DXSimp::Vector3& _position,
		const DXSimp::Vector3& _scale,
		const DXSimp::Quaternion& _rotation)
	{
		// 変換行列を作成
		// 移動行列
		DXSimp::Matrix t = DX::XMMatrixTranslation(
			_position.x,
			_position.y,
			_position.z
		);

		// スケーリング行列
		DXSimp::Matrix s = DX::XMMatrixScaling(
			_scale.x,
			_scale.y,
			_scale.z
		);

		// 回転行列
		DXSimp::Matrix r = DXSimp::Matrix::CreateFromQuaternion(_rotation);

		DXSimp::Matrix worldMtx = s * r * t;	// ワールド変換行列を作成

		wvp.world = worldMtx;

		return wvp;
	}

	const DirectX::SimpleMath::Matrix& RenderParam::GetProjectionMatrix() const
	{
		return wvp.projection;
	}
}