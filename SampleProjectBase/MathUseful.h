#pragma once

// �C�[�W���O
#include "Easing.hpp"

// ���w�I�ȕ֗��֐��i�撣���ĕ׋����I�j
namespace Mathf
{
	constexpr float roundDeg = 360.0f;	// 1���̊p�x
	constexpr float radToDeg = 57.2958f;	//�@���W�A�����p�x
	constexpr float degToRad = 0.0174533f;	//�@�p�x�����W�A��
	constexpr float epsilon = 1.1920e-7f;	// �ŏ�����
	constexpr float smallValue = 0.0001f;	// �������l
	constexpr float PI = std::numbers::pi_v<float>;	// ��

	/// @brief �l��0����ő�l�܂łŐ��K������
	/// @param _t ���K������l
	/// @param _length �ő�l
	/// @return ���K�����ꂽ�l
	float Repeat(float _t, float _length);
}

namespace Vec3
{
	/// <summary>
	/// ���ʐ��`��Ԃ�����֐�
	/// </summary>
	/// <param name="_startPos">�J�n�l</param>
	/// <param name="_endPos">�I�_�l</param>
	/// <param name="_ratio">����(0�`1)</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Slerp(
		DirectX::SimpleMath::Vector3 _startPos,
		DirectX::SimpleMath::Vector3 _endPos,
		float _ratio
	);
	// ���[���h���W�n3���P�ʃx�N�g��
	constexpr DirectX::SimpleMath::Vector3 Up = { 0.0f, 1.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Right = { 1.0f, 0.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Forward = { 0.0f, 0.0f, 1.0f };

	// ����
	float Dot(DirectX::SimpleMath::Vector3 _v1, DirectX::SimpleMath::Vector3 _v2);

	// �O��
	DirectX::SimpleMath::Vector3 Cross(DirectX::SimpleMath::Vector3 _v1,
		DirectX::SimpleMath::Vector3 _v2);

	/// @brief float�^�Ƃ̍ő�l�����߂�
	DirectX::SimpleMath::Vector3 Max(const DirectX::SimpleMath::Vector3& _v3, float _floatVal);
}

namespace Vec4
{
	/// @brief float�^�Ƃ̍ő�l�����߂�
	DirectX::SimpleMath::Vector4 Max(const DirectX::SimpleMath::Vector4& _v4, float _floatVal);
}

namespace Quat
{
	using namespace DirectX::SimpleMath;

	/// @brief �N�H�[�^�j�I�������]���Ɖ�]�p�x�����߂�
	/// @param _q �N�H�[�^�j�I��
	/// @param _axis ��]���̎Q��
	/// @param _angle ��]�p�x�̎Q��(rad)
	void ToAxisAngle(const Quaternion& _q, Vector3& _axis, float& _angle);

	/// @brief �N�H�[�^�j�I���̊|���Z������
	/// @param _q1 �N�H�[�^�j�I��1
	/// @param _q2 �N�H�[�^�j�I��2
	/// @return �|���Z�����N�H�[�^�j�I��
	Quaternion Multiply(const Quaternion& _q1, const Quaternion& _q2);
}