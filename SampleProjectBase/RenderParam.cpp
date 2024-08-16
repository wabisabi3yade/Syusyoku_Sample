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
	// �ϊ��s����쐬
	// �ړ��s��
	Matrix t = XMMatrixTranslation(
		_position.x,
		_position.y,
		_position.z
	);

	// �X�P�[�����O�s��
	Matrix s = XMMatrixScaling(
		_scale.x,
		_scale.y,
		_scale.z
	);

	// ��]�s��
	Matrix r = Matrix::CreateFromQuaternion(_rotation);

	Matrix worldMtx = s * r * t;	// ���[���h�ϊ��s����쐬

	worldMtx = worldMtx.Transpose();

	wvp.world = worldMtx;

	return wvp;
}

