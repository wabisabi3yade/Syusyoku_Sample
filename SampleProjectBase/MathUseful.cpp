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

DirectX::SimpleMath::Vector4 Vec4::Max(const DirectX::SimpleMath::Vector4& _v4, float _floatVal)
{
	Vector4 retV4;
	retV4.x = std::max<float>(_v4.x, _floatVal);
	retV4.y = std::max<float>(_v4.y, _floatVal);
	retV4.z = std::max<float>(_v4.z, _floatVal);
	retV4.w = std::max<float>(_v4.w, _floatVal);
	return retV4;
}

void Quat::ToAxisAngle(const DirectX::SimpleMath::Quaternion& _q, DirectX::SimpleMath::Vector3& _axis, float& _angle)
{
	// 回転角度の計算
	_angle = 2.0f * std::acos(_q.w);

	// sin(θ/2) の計算
	float sinHalfAngle = std::sqrt(1.0f - _q.w * _q.w);

	if (sinHalfAngle < 0.0001f) 
	{
		// 回転角度が非常に小さい場合、軸は任意の方向になり得る
		_axis = { 1.0f, 0.0f, 0.0f }; // 例えば、x軸を使用
	}
	else 
	{
		// 回転軸の計算
		_axis = { _q.x / sinHalfAngle, _q.y / sinHalfAngle, _q.z / sinHalfAngle };
	}
}

Quaternion Quat::Multiply(const Quaternion& _q1, const Quaternion& _q2)
{
	Quaternion result = _q1 * _q2;
	result.Normalize();

	return result;
}

float Mathf::Repeat(float _t, float _length)
{
	float f = fmod(_t, _length);

	if (f < 0.0f)
		f += _length;

	return f;
}
