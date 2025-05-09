#pragma once

namespace HashiTaku
{
	// 描画に必要なパラメータクラス
	class RenderParam
	{
	public:

		/// @brief ワールド・ビュー・プロジェクション	
		struct WVP
		{
			DXSimp::Matrix world;	// ワールド変換行列
			DXSimp::Matrix view;	// ビュー変換
			DXSimp::Matrix projection;	// プロジェクション
		};

	private:

		/// @brief ワールド変換行列
		WVP wvp;
	public:
		RenderParam() {}
		~RenderParam() {}

		WVP& GetWVP();

		/// @brief WVP変換行列を作成して渡す
		/// @param _transform 座標
		/// @param _scaleTimes スケール倍率
		/// @return wvp行列
		WVP& GetWVP(const Transform& _transform);

		/// @brief WVP変換行列を作成して渡す
		/// @param _position 座標
		/// @param _scale 大きさ
		/// @param _rotation 回転量
		/// @return wvp行列
		WVP& GetWVP(const DXSimp::Vector3& _position, const DXSimp::Vector3& _scale, const DXSimp::Quaternion& _rotation);

		/// @brief 現在の投影行列を取得
		/// @return 投影行列
		const DXSimp::Matrix& GetProjectionMatrix() const;

		/// @brief ビュー変換行列をセット
		/// @param _view ビュー変換行列
		void SetView(const DXSimp::Matrix& _view) { wvp.view = _view; }

		/// @brief プロジェクション変換行列をセット
		/// @param _proj プロジェクション変換行列
		void SetProjection(const DXSimp::Matrix& _proj) { wvp.projection = _proj; }
	};
}