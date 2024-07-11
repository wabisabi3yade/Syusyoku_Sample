#include "pch.h"
#include "MathUseful.h"

using namespace DirectX::SimpleMath;

Vector3 Vec3::Slerp(Vector3 _startPos, Vector3 _endPos, float _ratio)
{
	_ratio = std::clamp(_ratio, 0.0f, 1.0f);

	Vector3 retVec3;
	// ���ꂼ��̒P�ʃx�N�g�������߂�
	Vector3 s = _startPos;
	Vector3 e = _endPos;
	s.Normalize();
	e.Normalize();

	// �Q�x�N�g���Ԃ̊p�x�����߂�i�s�p���j
	// �A�[�N�R�T�C���͊p�x�����߂���
	float theta = acos(Vec3::Dot(s, e));
	// sin��
	float SinTh = sin(theta);

	// ��ԌW��
	float Ps = sin(theta * (1.0f - _ratio));
	float Pe = sin(theta * _ratio);
	retVec3 = (Ps * s + Pe * e) / SinTh;

	return retVec3;
}

float Vec3::Dot(DirectX::SimpleMath::Vector3 _v1, DirectX::SimpleMath::Vector3 _v2)
{
	return DirectX::XMVector3Dot(_v1, _v2).m128_f32[0];
}

DirectX::SimpleMath::Vector3 Vec3::Cross(DirectX::SimpleMath::Vector3 _v1, DirectX::SimpleMath::Vector3 _v2)
{
	return DirectX::XMVector3Cross(_v1, _v2);
}


