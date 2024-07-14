#include "pch.h"
#include "MathUseful.h"

using namespace DirectX::SimpleMath;

Vector3 Vec3::Slerp(Vector3 _startPos, Vector3 _endPos, float _ratio)
{
	_ratio = std::clamp(_ratio, 0.0f, 1.0f);

	Vector3 retVec3;
	// それぞれの単位ベクトルを求める
	Vector3 s = _startPos;
	Vector3 e = _endPos;
	s.Normalize();
	e.Normalize();

	// ２ベクトル間の角度を求める（鋭角側）
	// アークコサインは角度が求められる
	float theta = acos(Vec3::Dot(s, e));
	// sinθ
	float SinTh = sin(theta);

	// 補間係数
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

DirectX::SimpleMath::Vector3 Vec3::Max(const DirectX::SimpleMath::Vector3& _v3, float _floatVal)
{
	Vector3 retV3;
	retV3.x = std::max<float>(_v3.x, _floatVal);
	retV3.y = std::max<float>(_v3.y, _floatVal);
	retV3.z = std::max<float>(_v3.z, _floatVal);
	return retV3;
}


