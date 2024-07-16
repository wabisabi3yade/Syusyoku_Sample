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
	// �ϊ��s����쐬
	// �ړ��s��
	Matrix t = XMMatrixTranslation(
		_transform.position.x,
		_transform.position.y,
		_transform.position.z
	);

	// �X�P�[�����O�s��
	Matrix s = XMMatrixScaling(
		_transform.scale.x,
		_transform.scale.y,
		_transform.scale.z
	);

	// ��]�s��
	Matrix r = Matrix::CreateFromYawPitchRoll(
		_transform.rotation.y * Mathf::degToRad,
		_transform.rotation.x * Mathf::degToRad,
		_transform.rotation.z * Mathf::degToRad
	);

	Matrix worldMtx = s * r * t;	// ���[���h�ϊ��s����쐬

	worldMtx = worldMtx.Transpose();

	wvp.world = worldMtx;

	return wvp;
}

