#pragma once

// イージング
#include "Easing.hpp"

// 数学的な便利関数（頑張って勉強中！）
namespace Mathf
{
	constexpr float roundDeg = 360.0f;	// 1周の角度
	constexpr float radToDeg = 57.2958f;	//　ラジアン→角度
	constexpr float degToRad = 0.0174533f;	//　角度→ラジアン
	constexpr float epsilon = 1.1920e-7f;	// 最小正数
	constexpr float smallValue = 0.0001f;	// 小さい値
	constexpr float PI = std::numbers::pi_v<float>;	// π

	/// @brief 値を0から最大値までで正規化する
	/// @param _t 正規化する値
	/// @param _length 最大値
	/// @return 正規化された値
	float Repeat(float _t, float _length);
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
	// ワールド座標系3軸単位ベクトル
	constexpr DirectX::SimpleMath::Vector3 Up = { 0.0f, 1.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Right = { 1.0f, 0.0f, 0.0f };
	constexpr DirectX::SimpleMath::Vector3 Forward = { 0.0f, 0.0f, 1.0f };

	// 内積
	float Dot(DirectX::SimpleMath::Vector3 _v1, DirectX::SimpleMath::Vector3 _v2);

	// 外積
	DirectX::SimpleMath::Vector3 Cross(DirectX::SimpleMath::Vector3 _v1,
		DirectX::SimpleMath::Vector3 _v2);

	/// @brief float型との最大値を求める
	DirectX::SimpleMath::Vector3 Max(const DirectX::SimpleMath::Vector3& _v3, float _floatVal);
}

namespace Vec4
{
	/// @brief float型との最大値を求める
	DirectX::SimpleMath::Vector4 Max(const DirectX::SimpleMath::Vector4& _v4, float _floatVal);
}

namespace Quat
{
	using namespace DirectX::SimpleMath;

	/// @brief クォータニオンから回転軸と回転角度を求める
	/// @param _q クォータニオン
	/// @param _axis 回転軸の参照
	/// @param _angle 回転角度の参照(rad)
	void ToAxisAngle(const Quaternion& _q, Vector3& _axis, float& _angle);

	/// @brief クォータニオンの掛け算をする
	/// @param _q1 クォータニオン1
	/// @param _q2 クォータニオン2
	/// @return 掛け算したクォータニオン
	Quaternion Multiply(const Quaternion& _q1, const Quaternion& _q2);
}