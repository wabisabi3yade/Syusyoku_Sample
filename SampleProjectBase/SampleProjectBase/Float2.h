#pragma once
class Float2 : public DirectX::XMFLOAT2
{

public:
	// ‰Šú’l‘S‚Ä0
	Float2(float _x = 0, float _y = 0);

	/// <summary>
	/// 2‚Â‚ÌFloat2Œ^•Ï”‚Ì‹——£‚ğ‹‚ß‚é
	/// </summary>
	/// <param name="_v1">Float2Œ^‡@</param>
	/// <param name="_v2">Float2Œ^‡A</param>
	/// <returns></returns>
	static Float2 Distance(Float2 _v1, Float2 _v2);

	// ’è”
	static const Float2 one;	// (1,1,1)
	static const Float2 zero;	// (0,0,0)
	static const Float2 up;	// (0,1,0)
	static const Float2 down;	// (0,-1,0)
	static const Float2 right;	// (1,0,0)
	static const Float2 left;	// (-1,0,0)

	// ƒIƒyƒŒ[ƒ^[‰‰Zq
	// Vector“¯m‚Ìl‘¥‰‰Z
	Float2 operator +(Float2 _v);
	Float2 operator -(Float2 _v);
	Float2 operator *(Float2 _v);
	Float2 operator /(Float2 _v);

	// floatŒ^‚Æ‚ÌÏEŠ„
	Float2 operator *(float _f);
	Float2 operator /(float _f);

	// ğŒ®
	Float2 operator ==(Float2 _v);
	Float2 operator !=(Float2 _v);
};

