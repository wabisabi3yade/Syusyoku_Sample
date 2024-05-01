#pragma once
class Float2 : public DirectX::XMFLOAT2
{

public:
	// �����l�S��0
	Float2(float _x = 0, float _y = 0);

	/// <summary>
	/// 2��Float2�^�ϐ��̋��������߂�
	/// </summary>
	/// <param name="_v1">Float2�^�@</param>
	/// <param name="_v2">Float2�^�A</param>
	/// <returns></returns>
	static Float2 Distance(Float2 _v1, Float2 _v2);

	// �萔
	static const Float2 one;	// (1,1,1)
	static const Float2 zero;	// (0,0,0)
	static const Float2 up;	// (0,1,0)
	static const Float2 down;	// (0,-1,0)
	static const Float2 right;	// (1,0,0)
	static const Float2 left;	// (-1,0,0)

	// �I�y���[�^�[���Z�q
	// Vector���m�̎l�����Z
	Float2 operator +(Float2 _v);
	Float2 operator -(Float2 _v);
	Float2 operator *(Float2 _v);
	Float2 operator /(Float2 _v);

	// float�^�Ƃ̐ρE��
	Float2 operator *(float _f);
	Float2 operator /(float _f);

	// ������
	Float2 operator ==(Float2 _v);
	Float2 operator !=(Float2 _v);
};

