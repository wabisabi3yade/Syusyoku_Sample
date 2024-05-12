#include "pch.h"
#include "Float3.h"

// ’è”‚Ì’è‹`
const Float3 Float3::one = { 1.0f, 1.0f, 1.0f };
const Float3 Float3::zero = {};
const Float3 Float3::up = { 0.0f, 1.0f, 0.0f };
const Float3 Float3::down = { 0.0f, -1.0f, 0.0f };
const Float3 Float3::right = { 1.0f, 0.0f, 0.0f };
const Float3 Float3::left = { -1.0f, 0.0f, 0.0f };
const Float3 Float3::forward = { 0.0f, 0.0f, 1.0f };
const Float3 Float3::back = { 0.0f, 0.0f, -1.0f };

Float3::Float3(float _x, float _y, float _z) : DirectX::XMFLOAT3(_x, _y, _z)
{
}

Float3::Float3(DirectX::XMFLOAT4 _f)
{
	this->x = _f.x; this->y = _f.y; this->z = _f.z;
}

Float3 Float3::Distance(Float3 _v1, Float3 _v2)
{
	Float3 d = _v1 - _v2;

	return sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
}

Float3 Float3::operator+(Float3 _v)
{
	return { this->x + _v.x, this->y + _v.y, this->z + _v.z };
}
Float3 Float3::operator-(Float3 _v)
{
	return { this->x - _v.x, this->y - _v.y, this->z - _v.z };
}
Float3 Float3::operator*(Float3 _v)
{
	return { this->x * _v.x, this->y * _v.y, this->z * _v.z };
}
Float3 Float3::operator/(Float3 _v)
{
	return { this->x / _v.x, this->y / _v.y, this->z / _v.z };
}

Float3 Float3::operator*(float _f)
{
	return { this->x * _f, this->y * _f, this->z * _f };
}
Float3 Float3::operator/(float _f)
{
	return { this->x / _f, this->y / _f, this->z / _f };
}

Float3 Float3::operator==(Float3 _v)
{
	bool isRet =
		(this->x == _v.x && this->y == _v.y && this->z == _v.z);

	return isRet;
}
Float3 Float3::operator!=(Float3 _v)
{
	bool isRet =
		(this->x != _v.x || this->y != _v.y || this->z != _v.z);

	return isRet;
}

