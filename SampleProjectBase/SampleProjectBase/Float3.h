#pragma once

// floatŒ^•Ï”3‚Â‚ÌƒNƒ‰ƒX(XMFloat3Œ^‚ğŒp³)
class Float3 : public DirectX::XMFLOAT3
{
public:
	// ‰Šú’l‘S‚Ä0
	Float3(float _x = 0, float _y = 0, float _z = 0);
	Float3(const Float3&) = default;
	Float3(DirectX::XMFLOAT4 _f);

	// ’è”
	static const Float3 one;	// (1,1,1)
	static const Float3 zero;	// (0,0,0)
	static const Float3 up;	// (0,1,0)
	static const Float3 down;	// (0,-1,0)
	static const Float3 right;	// (1,0,0)
	static const Float3 left;	// (-1,0,0)
	static const Float3 forward;	// (0,0,1)
	static const Float3 back;	// (0,0,-1)

	// ƒ}ƒNƒŠÖ”

	/// <summary>
	/// 2‚Â‚ÌFloat3Œ^•Ï”‚Ì‹——£‚ğ‹‚ß‚é
	/// </summary>
	/// <param name="_v1">Float3Œ^‡@</param>
	/// <param name="_v2">Float3Œ^‡A</param>
	/// <returns></returns>
	static Float3 Distance(Float3 _v1, Float3 _v2);

	// ƒIƒyƒŒ[ƒ^[‰‰Zq
	// Vector“¯m‚Ìl‘¥‰‰Z
	Float3 operator +(Float3 _v);
	Float3 operator -(Float3 _v);
	Float3 operator *(Float3 _v);
	Float3 operator /(Float3 _v);

	// floatŒ^‚Æ‚ÌÏEŠ„
	Float3 operator *(float _f);
	Float3 operator /(float _f);

	// ğŒ®
	Float3 operator ==(Float3 _v);
	Float3 operator !=(Float3 _v);	
};

