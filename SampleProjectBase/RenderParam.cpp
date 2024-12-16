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
		// �ϊ��s����쐬
		// �ړ��s��
		DXSimp::Matrix t = DX::XMMatrixTranslation(
			_position.x,
			_position.y,
			_position.z
		);

		// �X�P�[�����O�s��
		DXSimp::Matrix s = DX::XMMatrixScaling(
			_scale.x,
			_scale.y,
			_scale.z
		);

		// ��]�s��
		DXSimp::Matrix r = DXSimp::Matrix::CreateFromQuaternion(_rotation);

		DXSimp::Matrix worldMtx = s * r * t;	// ���[���h�ϊ��s����쐬

		wvp.world = worldMtx;

		return wvp;
	}

	const DirectX::SimpleMath::Matrix& RenderParam::GetProjectionMatrix() const
	{
		return wvp.projection;
	}
}