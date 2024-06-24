#pragma once

// ���w�I�ȕ֗��֐��i�撣���ĕ׋����I�j
namespace Mathf
{
	constexpr float roundDeg = 360.0f;	// 1���̊p�x
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
}
