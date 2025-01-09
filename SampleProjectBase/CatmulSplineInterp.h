#pragma once

namespace HashiTaku
{
	/// @brief アニメーションで使用するベジェ補間
	class CatmulSplineInterp
	{
		CatmulSplineInterp() {}
		~CatmulSplineInterp() {}
	public:
		/// @brief Catmull-Rom スプラインによる制御点と接線ベクトルの計算
		/// @param _from 補間したパラメータ元
		/// @param _to 補間したパラメータ先
		/// @param _prev 補間元の一つ前のパラメータ
		/// @param _next 補間先の一つ先のパラメータ

		/// @brief  Catmullスプラインによる補間
		/// @param _from パラメータ元
		/// @param _to パラメータ先
		/// @param _prev パラメータ元から一つ前のパラメータ
		/// @param _next パラメータ先の一つ先のパラメータ
		/// @param _ratio 割合
		/// @return 計算値
		static DXSimp::Vector3 CalcVector3(const DXSimp::Vector3& _from, const DXSimp::Vector3& _to, const DXSimp::Vector3& _prev, const DXSimp::Vector3& _next, float _ratio);


		static DXSimp::Quaternion CalcQuaternion(const DXSimp::Quaternion& _from, const DXSimp::Quaternion& _to, const DXSimp::Quaternion& _prev, const DXSimp::Quaternion& next, float _ratio);

	private:

		/// @brief Catmull-Rom スプラインによる制御点と接線ベクトルの計算
		/// @param _from 補間したパラメータ元
		/// @param _to 補間したパラメータ先
		/// @param _prev 補間元の一つ前のパラメータ
		/// @param _next 補間先の一つ先のパラメータ
		/// @param _point1 導出した制御点1
		/// @param _point2 導出した制御点2
		/*static void CalcControlPoints(const DXSimp::Vector3& _from, const DXSimp::Vector3& _to, const DXSimp::Vector3& _prev, const DXSimp::Vector3& _next, DXSimp::Vector3& _point1, DXSimp::Vector3& _point2);*/
	};
}