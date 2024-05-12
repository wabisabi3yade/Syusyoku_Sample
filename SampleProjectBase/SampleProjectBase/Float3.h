#pragma once

// float^ฯ3ยฬNX(XMFloat3^๐pณ)
class Float3 : public DirectX::XMFLOAT3
{
public:
	// ๚lSฤ0
	Float3(float _x = 0, float _y = 0, float _z = 0);
	Float3(const Float3&) = default;
	Float3(DirectX::XMFLOAT4 _f);

	// ่
	static const Float3 one;	// (1,1,1)
	static const Float3 zero;	// (0,0,0)
	static const Float3 up;	// (0,1,0)
	static const Float3 down;	// (0,-1,0)
	static const Float3 right;	// (1,0,0)
	static const Float3 left;	// (-1,0,0)
	static const Float3 forward;	// (0,0,1)
	static const Float3 back;	// (0,0,-1)

	// }Nึ

	/// <summary>
	/// 2ยฬFloat3^ฯฬฃ๐฿้
	/// </summary>
	/// <param name="_v1">Float3^@</param>
	/// <param name="_v2">Float3^A</param>
	/// <returns></returns>
	static Float3 Distance(Float3 _v1, Float3 _v2);

	// Iy[^[Zq
	// VectorฏmฬlฅZ
	Float3 operator +(Float3 _v);
	Float3 operator -(Float3 _v);
	Float3 operator *(Float3 _v);
	Float3 operator /(Float3 _v);

	// float^ฦฬฯE
	Float3 operator *(float _f);
	Float3 operator /(float _f);

	// ๐ฎ
	Float3 operator ==(Float3 _v);
	Float3 operator !=(Float3 _v);	
};

