#include "pch.h"
#include "Float4.h"

const Float4 Float4::one = { 1.0f,1.0f,1.0f,1.0f };	// (1,1,1,1)
const Float4 Float4::zero = { 0.0f,0.0f,0.0f,0.0f };	// (0,0,0,0)

Float4::Float4(float _x, float _y, float _z, float _w)
	: DirectX::XMFLOAT4(_x, _y, _z, _w)
{
}

Float4 Float4::operator+(Float4 _v)
{
	return { this->x + _v.x, this->y + _v.y, this->z + _v.z, this->w + _v.w };
}

Float4 Float4::operator-(Float4 _v)
{
	return { this->x - _v.x, this->y - _v.y, this->z - _v.z, this->w - _v.w };
}

Float4 Float4::operator*(Float4 _v)
{
	return { this->x * _v.x, this->y * _v.y, this->z * _v.z, this->w * _v.w };
}

Float4 Float4::operator/(Float4 _v)
{
	return { this->x / _v.x, this->y / _v.y, this->z / _v.z, this->w / _v.w };
}

Float4 Float4::operator*(float _f)
{
	return { this->x * _f, this->y * _f, this->z * _f , this->w * _f };
}

Float4 Float4::operator/(float _f)
{
	return { this->x / _f, this->y / _f, this->z / _f , this->w / _f };
}

Float4 Float4::operator==(Float4 _v)
{
	bool isRet =
		(this->x == _v.x && this->y == _v.y && this->z == _v.z && this->w == _v.w);

	return isRet;
}

Float4 Float4::operator!=(Float4 _v)
{
	bool isRet =
		(this->x != _v.x || this->y != _v.y || this->z != _v.z || this->w != _v.w);

	return isRet;
}
