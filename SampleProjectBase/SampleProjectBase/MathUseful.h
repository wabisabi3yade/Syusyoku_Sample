#pragma once

// 数学的な便利関数（頑張って勉強中！）
namespace Mathf
{
	constexpr float roundDeg = 360.0f;	// 1周の角度
	constexpr float radToDeg = 57.2958f;	//　ラジアン→角度
	constexpr float degToRad = 0.0174533f;	//　角度→ラジアン
}

namespace Vec3
{
	/// <summary>
	/// 球面線形補間をする関数
	/// </summary>
	/// <param name="_startPos">開始値</param>
	/// <param name="_endPos">終点値</param>
	/// <param name="_ratio">割合(0～1)</param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Slerp(
		DirectX::SimpleMath::Vector3 _startPos,
		DirectX::SimpleMath::Vector3 _endPos,
		float _ratio
	);
}
