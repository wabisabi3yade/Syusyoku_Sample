#include "pch.h"
#include "Float2.h"

// ’è”‚Ì’è‹`
const Float2 Float2::one = { 1.0f, 1.0f };
const Float2 Float2::zero = {};
const Float2 Float2::up = { 0.0f, 1.0f };
const Float2 Float2::down = { 0.0f, -1.0f };
const Float2 Float2::right = { 1.0f, 0.0f };
const Float2 Float2::left = { -1.0f, 0.0f };


Float2::Float2(float _x, float _y) : DirectX::XMFLOAT2(_x, _y)
{
}

Float2::Float2(DirectX::XMFLOAT2 _f)
{
	this->x = _f.x; this->y = _f.y;
}

Float2 Float2::Distance(Float2 _v1, Float2 _v2)
{
	Float2 d = _v1 - _v2;

	return sqrt(d.x * d.x + d.y * d.y);
}

Float2 Float2::operator+(Float2 _v)
{
	return { this->x + _v.x, this->y + _v.y };
}
Float2 Float2::operator-(Float2 _v)
{
	return { this->x - _v.x, this->y - _v.y };
}
Float2 Float2::operator*(Float2 _v)
{
	return { this->x * _v.x, this->y * _v.y };
}
Float2 Float2::operator/(Float2 _v)
{
	return { this->x / _v.x, this->y / _v.y };
}

Float2 Float2::operator*(float _f)
{
	return { this->x * _f, this->y * _f };
}
Float2 Float2::operator/(float _f)
{
	return { this->x / _f, this->y / _f };
}

Float2 Float2::operator==(Float2 _v)
{
	bool isRet =
		(this->x == _v.x && this->y == _v.y);

	return isRet;
}
Float2 Float2::operator!=(Float2 _v)
{
	bool isRet =
		(this->x != _v.x || this->y != _v.y);

	return isRet;
}