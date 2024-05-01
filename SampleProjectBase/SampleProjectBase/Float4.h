#pragma once
class Float4 : public DirectX::XMFLOAT4
{
public:
	// ๚lSฤ0
	Float4(float _x = 0, float _y = 0, float _z = 0, float _w = 0);

	// ่
	static const Float4 one;	// (1,1,1,1)
	static const Float4 zero;	// (0,0,0,0)

	// }Nึ

	// Iy[^[Zq
	// VectorฏmฬlฅZ
	Float4 operator +(Float4 _v);
	Float4 operator -(Float4 _v);
	Float4 operator *(Float4 _v);
	Float4 operator /(Float4 _v);

	// float^ฦฬฯE
	Float4 operator *(float _f);
	Float4 operator /(float _f);

	// ๐ฎ
	Float4 operator ==(Float4 _v);
	Float4 operator !=(Float4 _v);
};

