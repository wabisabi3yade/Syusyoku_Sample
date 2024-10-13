#include "pch.h"
#include "MathUseful.h"

using namespace DirectX;
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

DirectX::SimpleMath::Vector3 Vec3::Max(const DirectX::SimpleMath::Vector3& _v3, float _floatVal)
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

DirectX::SimpleMath::Vector3 Vec3::WorldMtxToScale(const DirectX::SimpleMath::Matrix& _mtx)
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
	// ��]�p�x�̌v�Z
	_angle = 2.0f * std::acos(_q.w);

	// sin(��/2) �̌v�Z
	float sinHalfAngle = 1.0f - _q.w * _q.w;

	if (sinHalfAngle <= Mathf::epsilon)
	{
		// ��]�p�x�����ɏ������ꍇ�A���͔C�ӂ̕����ɂȂ蓾��
		_axis = { 1.0f, 0.0f, 0.0f }; // �Ⴆ�΁Ax�����g�p
	}
	else
	{
		float sqrtHalfAngle = sqrt(sinHalfAngle);
		// ��]���̌v�Z
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

	// �E�x�N�g��
	Vector3 right = _up.Cross(forward);
	right.Normalize();

	// ��x�N�g��
	Vector3 up = forward.Cross(right);
	up.Normalize();

	// ��]�s��
	Matrix rotate = Mtx::RotateMatrixByVector(right, up, forward);

	// �N�H�[�^�j�I������	
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

	// �N�H�[�^�j�I���ɔ��f������
	q = Quaternion::CreateFromYawPitchRoll(
		_degrees.y * Mathf::degToRad,
		_degrees.x * Mathf::degToRad,
		_degrees.z * Mathf::degToRad
	);

	return q;
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

	// matrix[0] �� �E����
	right.x = _matrix.m[0][0];
	right.y = _matrix.m[0][1];
	right.z = _matrix.m[0][2];

	return right;
}

Vector3 Mtx::GetUpVector(const Matrix& _matrix)
{
	Vector3 up;

	// matrix[1] �� �㐬��
	up.x = _matrix.m[1][0];
	up.y = _matrix.m[1][1];
	up.z = _matrix.m[1][2];

	return up;
}

Vector3 Mtx::GetForwardVector(const Matrix& _matrix)
{
	Vector3 forward;

	// matrix[2] �� �O����
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

void Mtx::GetTransformFromWldMtx(const Matrix& _worldMtx, Vector3& _pos, Vector3& _scele, Quaternion& _rot)
{
	// �s�񂩂�g�����X�t�H�[�������߂�
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
