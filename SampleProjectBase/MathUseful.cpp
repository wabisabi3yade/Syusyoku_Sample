#include "pch.h"
#include "MathUseful.h"

namespace HashiTaku
{
	using namespace DXSimp;

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

	float Vec3::Dot(DXSimp::Vector3 _v1, DXSimp::Vector3 _v2)
	{
		return DirectX::XMVector3Dot(_v1, _v2).m128_f32[0];
	}

	DXSimp::Vector3 Vec3::Cross(DXSimp::Vector3 _v1, DXSimp::Vector3 _v2)
	{
		return DirectX::XMVector3Cross(_v1, _v2);
	}

	DXSimp::Vector3 Vec3::Max(const DXSimp::Vector3& _v3, float _floatVal)
	{
		Vector3 retV3;
		retV3.x = std::max<float>(_v3.x, _floatVal);
		retV3.y = std::max<float>(_v3.y, _floatVal);
		retV3.z = std::max<float>(_v3.z, _floatVal);
		return retV3;
	}

	Vector3 Vec3::Abs(const Vector3& _v)
	{
		Vector3 retV;

		retV.x = abs(_v.x);
		retV.y = abs(_v.y);
		retV.z = abs(_v.z);

		return retV;
	}

	DXSimp::Vector3 Vec3::WorldMtxToScale(const DXSimp::Matrix& _mtx)
	{
		Vector3 scale;

		scale.x = Vector3(_mtx._11, _mtx._12, _mtx._13).Length();
		scale.y = Vector3(_mtx._21, _mtx._22, _mtx._23).Length();
		scale.z = Vector3(_mtx._31, _mtx._32, _mtx._33).Length();

		return scale;
	}

	Vector3 Vec3::Repeat(const Vector3& _v, float _length)
	{
		return Vector3(
			Mathf::Repeat(_v.x, _length),
			Mathf::Repeat(_v.y, _length),
			Mathf::Repeat(_v.z, _length)
		);
	}

	std::string Vec3::ToStr(const Vector3& _v)
	{
		return "x:" + std::to_string(_v.x)
			+ " y:" + std::to_string(_v.y)
			+ " z:" + std::to_string(_v.z);
	}

	DXSimp::Vector4 Vec4::Max(const DXSimp::Vector4& _v4, float _floatVal)
	{
		Vector4 retV4;
		retV4.x = std::max<float>(_v4.x, _floatVal);
		retV4.y = std::max<float>(_v4.y, _floatVal);
		retV4.z = std::max<float>(_v4.z, _floatVal);
		retV4.w = std::max<float>(_v4.w, _floatVal);
		return retV4;
	}

	void Quat::ToAxisAngle(const DXSimp::Quaternion& _q, DXSimp::Vector3& _axis, float& _angle)
	{
		// 回転角度の計算
		_angle = 2.0f * std::acos(_q.w);

		// sin(θ/2) の計算
		float sinHalfAngle = 1.0f - _q.w * _q.w;

		if (sinHalfAngle <= Mathf::epsilon)
		{
			// 回転角度が非常に小さい場合、軸は任意の方向になり得る
			_axis = { 1.0f, 0.0f, 0.0f }; // 例えば、x軸を使用
		}
		else
		{
			float sqrtHalfAngle = sqrt(sinHalfAngle);
			// 回転軸の計算
			_axis = { _q.x / sqrtHalfAngle, _q.y / sqrtHalfAngle, _q.z / sqrtHalfAngle };
		}
	}

	Quaternion Quat::Multiply(const Quaternion& _q1, const Quaternion& _q2)
	{
		Quaternion result = _q1 * _q2;
		result.Normalize();

		return result;
	}

	Quaternion Quat::RotationDifference(const Quaternion& _origin, const Quaternion& _sub)
	{
		Quaternion invSub;
		_sub.Inverse(invSub);

		return Quat::Multiply(_origin, invSub);
	}

	Quaternion Quat::RotateToVector(const Vector3& _vector, const Vector3& _up)
	{
		Vector3 forward;
		_vector.Normalize(forward);

		// 右ベクトル
		Vector3 right = _up.Cross(forward);
		right.Normalize();

		// 上ベクトル
		Vector3 up = forward.Cross(right);
		up.Normalize();

		// 回転行列
		Matrix rotate = Mtx::RotateMatrixByVector(right, up, forward);

		// クォータニオン生成	
		Quaternion quaternion = Quaternion::CreateFromRotationMatrix(rotate);

		return quaternion;
	}

	Vector3 Quat::ToEulerAngles(const Quaternion& _q)
	{
		return _q.ToEuler() * Mathf::radToDeg;
	}

	Quaternion Quat::ToQuaternion(const Vector3& _degrees)
	{
		Quaternion q;

		// クォータニオンに反映させる
		q = Quaternion::CreateFromYawPitchRoll(
			_degrees.y * Mathf::degToRad,
			_degrees.x * Mathf::degToRad,
			_degrees.z * Mathf::degToRad
		);

		return q;
	}

	bool Quat::CheckEqual(const Quaternion& _q1, const Quaternion& _q2)
	{
		// 内積で1ならば同等と扱う
		float dot = _q1.Dot(_q2);
		
		return std::abs(dot) > 1.0f - Mathf::epsilon;
	}

	std::string Quat::ToStr(const Quaternion& _q)
	{
		return "x:" + std::to_string(_q.x)
			+ " y:" + std::to_string(_q.y)
			+ " z:" + std::to_string(_q.z)
			+ " w:" + std::to_string(_q.w);
	}

	float Mathf::Repeat(float _t, float _length)
	{
		float f = fmod(_t, _length);

		if (f < 0.0f)
			f += _length;

		return f;
	}

	float Mathf::Lerp(float _from, float _to, float _ratio)
	{
		_ratio = std::clamp(_ratio, 0.0f, 1.0f);
		float sub = _to - _from;
		return _from + sub * _ratio;
	}

	Vector3 Mtx::GetRightVector(const Matrix& _matrix)
	{
		Vector3 right;

		// matrix[0] → 右成分
		right.x = _matrix.m[0][0];
		right.y = _matrix.m[0][1];
		right.z = _matrix.m[0][2];

		return right;
	}

	Vector3 Mtx::GetUpVector(const Matrix& _matrix)
	{
		Vector3 up;

		// matrix[1] → 上成分
		up.x = _matrix.m[1][0];
		up.y = _matrix.m[1][1];
		up.z = _matrix.m[1][2];

		return up;
	}

	Vector3 Mtx::GetForwardVector(const Matrix& _matrix)
	{
		Vector3 forward;

		// matrix[2] → 前成分
		forward.x = _matrix.m[2][0];
		forward.y = _matrix.m[2][1];
		forward.z = _matrix.m[2][2];

		return forward;
	}

	Matrix Mtx::RotateMatrixByVector(const Vector3& _right, const Vector3& _up, const Vector3& _forward)
	{
		Matrix rotation =
			Matrix(_right.x, _right.y, _right.z, 0.0f,
				_up.x, _up.y, _up.z, 0.0f,
				_forward.x, _forward.y, _forward.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

		return rotation;
	}

	Matrix Mtx::CreateRoratateMtx(const Vector3& _degrees)
	{
		return Matrix::CreateFromYawPitchRoll(
			_degrees.y * Mathf::degToRad,
			_degrees.x * Mathf::degToRad,
			_degrees.z * Mathf::degToRad
		);
	}

	void Mtx::CreateTransformMatrix(const Vector3& _pos, const Vector3& _scale, const Quaternion& _rotation, Matrix& _outMatrix)
	{
		// 座標移動
		Matrix transformMtx = Matrix::CreateTranslation(_pos);

		// スケール
		Matrix scaleMtx = Matrix::CreateScale(_scale);

		// 回転量
		Quaternion normalize;
		_rotation.Normalize(normalize);
		Matrix rotMtx = Matrix::CreateFromQuaternion(normalize);

		// SRT
		_outMatrix = scaleMtx * rotMtx * transformMtx;
	}

	void Mtx::GetTransformFromWldMtx(const Matrix& _worldMtx, Vector3& _pos, Vector3& _scele, Quaternion& _rot)
	{
		// 行列からトランスフォームを求める
		_pos = {
			_worldMtx._41,
			_worldMtx._42,
			_worldMtx._43
		};

		_scele = Vec3::WorldMtxToScale(_worldMtx);

		Matrix rotateMtx = Matrix(
			_worldMtx._11 / _scele.x, _worldMtx._12 / _scele.x, _worldMtx._13 / _scele.x, 0.0f,
			_worldMtx._21 / _scele.y, _worldMtx._22 / _scele.y, _worldMtx._23 / _scele.y, 0.0f,
			_worldMtx._31 / _scele.z, _worldMtx._32 / _scele.z, _worldMtx._33 / _scele.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		_rot = Quaternion::CreateFromRotationMatrix(rotateMtx);
	}

	Vector3 Mtx::MultiplyVector3(const Matrix& _mtx, const Vector3& _v)
	{
		return XMVector3Transform(_v, _mtx);
	}

	Vector2 Vec2::Abs(const Vector2& _v)
	{
		Vector2 retV;

		retV.x = abs(_v.x);
		retV.y = abs(_v.y);

		return retV;
	}

	std::string Vec2::ToStr(const Vector2& _v)
	{
		return "x:" + std::to_string(_v.x)
			+ " y:" + std::to_string(_v.y);
	}
}