#pragma once

// イージング
#include "Easing.hpp"

namespace HashiTaku
{
	// 数学的な便利関数（頑張って勉強中！）
	namespace Mathf
	{
		constexpr float roundDeg = 360.0f;	// 1周の角度
		constexpr float radToDeg = 57.2958f;	//　ラジアン→角度
		constexpr float degToRad = 0.0174533f;	//　角度→ラジアン
		constexpr float epsilon = std::numeric_limits<float>::epsilon();	// 最小正数
		constexpr float smallValue = 0.000001f;	// 小さい値
		constexpr float PI = std::numbers::pi_v<float>;	// 

		/// @brief 値を0から最大値までで正規化する
		/// @param _t 正規化する値
		/// @param _length 最大値
		/// @return 正規化された値
		float Repeat(float _t, float _length);

		/// @brief 割合による線形補間
		/// @param _from 元
		/// @param _to 先
		/// @param _ratio 割合 
		/// @return 計算値
		float Lerp(float _from, float _to, float _ratio);
	}

	namespace Vec2
	{
		using namespace DXSimp;
		/// @brief 絶対値を求める
		/// @param _v 変数
		/// @return 絶対値
		Vector2 Abs(const Vector2& _v);

		/// @brief 文字列に変換する
		/// @param _v Vector2
		/// @return 文字列
		std::string ToStr(const Vector2& _v);
	}

	namespace Vec3
	{
		using namespace DXSimp;

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

		/// @brief ワールド行列からスケールを求める
		/// @param _mtx ワールド行列
		/// @return スケール値
		DXSimp::Vector3 WorldMtxToScale(const DXSimp::Matrix& _mtx);

		/// @brief 値を0から最大値までで正規化する
		/// @param _v 正規化する値
		/// @param _length 最大値
		/// @return 正規化された値
		Vector3 Repeat(const Vector3& _v, float _length);

		/// @brief 文字列に変換する
		/// @param _v Vector3
		/// @return 文字列
		std::string ToStr(const Vector3& _v);
	}

	namespace Vec4
	{
		/// @brief float型との最大値を求める
		DXSimp::Vector4 Max(const DXSimp::Vector4& _v4, float _floatVal);
	}

	namespace Quat
	{
		using namespace DXSimp;

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

		/// @brief クォータニオンの差分を求める
		/// @param _origin 元
		/// @param _sub 引き算
		/// @return 差分
		Quaternion RotationDifference(const Quaternion& _origin, const Quaternion& _sub);

		/// @brief ターゲットの方向に向けるクォータニオンを生成する
		/// @param _vector 向けたいベクトル
		/// @param _up 上方向ベクトル
		/// @return 向けるクォータニオン
		Quaternion RotateToVector(const Vector3& _vector, const Vector3& _up = Vec3::Up);

		/// @brief クォータニオンからオイラー角に変換
		/// @param _q クォータニオン
		/// @return オイラー角
		Vector3 ToEulerAngles(const Quaternion& _q);

		/// @brief 角度からクォータニオン生成
		/// @param _degrees 回転角度(deg)
		/// @return クォータニオン
		Quaternion ToQuaternion(const Vector3& _degrees);

		/// @brief ほぼ等しいクォータニオンか判断する
		/// @return 等しいか？
		bool CheckEqual(const Quaternion& _q1, const Quaternion& _q2);

		/// @brief 文字列に変換する
		/// @param _v Quaternion
		/// @return 文字列
		std::string ToStr(const Quaternion& _q);
	}

	namespace Mtx
	{
		using namespace DXSimp;

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
		Matrix RotateMatrixByVector(const Vector3& _right, const Vector3& _up, const Vector3& _forward);

		/// @brief 角度から回転行列を求める
		/// @param _degrees 角度(deg) 
		/// @return 回転行列
		Matrix CreateRoratateMtx(const Vector3& _degrees);

		/// @brief トランスフォームから行列を作成する
		/// @param _transform トランスフォーム
		/// @param _outtMatrix 結果行列
		void CreateTransformMatrix(const Vector3& _pos, const Vector3& _scale, const Quaternion& _rotation, Matrix& _outMatrix);

		/// @brief ワールド行列から座標・スケール・回転量を取得
		/// @param _worldMtx ワールド行列
		/// @param _pos 移動座標
		/// @param _scele スケール
		/// @param _rot 回転量		
		void GetTransformFromWldMtx(const Matrix& _worldMtx, Vector3& _pos, Vector3& _scele, Quaternion& _rot);

		/// @brief Vector3と掛け算をする
		/// @param _mtx 行列
		/// @param _v Vector3
		/// @return 計算したVector3
		Vector3 MultiplyVector3(const Matrix& _mtx, const Vector3& _v);
	}
}