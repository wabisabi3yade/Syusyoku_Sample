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

namespace Vec2
{
	using namespace DirectX::SimpleMath;
	/// @brief 絶対値を求める
	/// @param _v 変数
	/// @return 絶対値
	Vector2 Abs(const Vector2& _v);
}

namespace Vec3
{
	using namespace DirectX::SimpleMath;

	/// <summary>
	/// 球面線形補間をする関数
	/// </summary>
	/// <param name="_startPos">開始値</param>
	/// <param name="_endPos">終点値</param>
	/// <param name="_ratio">割合(0～1)</param>
	/// <returns></returns>
	Vector3 Slerp(
		Vector3 _startPos,
		Vector3 _endPos,
		float _ratio
	);
	// ワールド座標系3軸単位ベクトル
	constexpr Vector3 Up = { 0.0f, 1.0f, 0.0f };
	constexpr Vector3 Right = { 1.0f, 0.0f, 0.0f };
	constexpr Vector3 Forward = { 0.0f, 0.0f, 1.0f };

	// 内積
	float Dot(Vector3 _v1, Vector3 _v2);

	// 外積
	Vector3 Cross(Vector3 _v1,
		Vector3 _v2);

	/// @brief float型との最大値を求める
	Vector3 Max(const Vector3& _v3, float _floatVal);

	/// @brief 絶対値を求める
	/// @param _v 変数
	/// @return 絶対値
	Vector3 Abs(const Vector3& _v);
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

	/// @brief ターゲットの方向に向けるクォータニオンを生成する
	/// @param _vector 向けたいベクトル
	/// @param _up 上方向ベクトル
	/// @return 向けるクォータニオン
	Quaternion RotateToVector(const Vector3& _vector, const Vector3& _up = Vec3::Up);

	/// @brief クォータニオンからオイラー角に変換
	/// @param _q クォータニオン
	/// @return オイラー角
	Vector3 ToEulerAngles(const Quaternion& _q);
}

namespace Mtx
{
	using namespace DirectX::SimpleMath;

	// 真右ベクトル取得
	Vector3 GetRightVector(const Matrix& _matrix);

	// 真上ベクトル取得
	Vector3 GetUpVector(const Matrix& _matrix);

	// 正面ベクトル取得
	Vector3 GetForwardVector(const Matrix& _matrix);

	/// @brief 方向ベクトルから回転行列作成
	/// @param _right 右
	/// @param _up 上
	/// @param _forward 前
	/// @return 回転行列
	Matrix RotateMatrixByVecdtor(const Vector3& _right, const Vector3& _up, const Vector3& _forward);
}