#pragma once

// ���w�I�ȕ֗��֐��i�撣���ĕ׋����I�j
namespace Mathf
{
	constexpr float roundDeg = 360.0f;	// 1���̊p�x
	constexpr float radToDeg = 57.2958f;	//�@���W�A�����p�x
	constexpr float degToRad = 0.0174533f;	//�@�p�x�����W�A��
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
}