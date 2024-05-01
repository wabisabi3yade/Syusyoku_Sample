#pragma once
class Float4 : public DirectX::XMFLOAT4
{
public:
	// 初期値全て0
	Float4(float _x = 0, float _y = 0, float _z = 0, float _w = 0);

	// 定数
	static const Float4 one;	// (1,1,1,1)
	static const Float4 zero;	// (0,0,0,0)

	// マクロ関数

	// オペレーター演算子
	// Vector同士の四則演算
	Float4 operator +(Float4 _v);
	Float4 operator -(Float4 _v);
	Float4 operator *(Float4 _v);
	Float4 operator /(Float4 _v);

	// float型との積・割
	Float4 operator *(float _f);
	Float4 operator /(float _f);

	// 条件式
	Float4 operator ==(Float4 _v);
	Float4 operator !=(Float4 _v);
};

