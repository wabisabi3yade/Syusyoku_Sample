#pragma once

// 描画に必要なパラメータクラス
class RenderParam
{
public:

	/// @brief ワールド・ビュー・プロジェクション	
	struct WVP	
	{
		DirectX::SimpleMath::Matrix world;	// ワールド変換行列
		DirectX::SimpleMath::Matrix view;	// ビュー変換
		DirectX::SimpleMath::Matrix projection;	// プロジェクション
	};

private:

	/// @brief ワールド変換行列
	WVP wvp;	
public:
	RenderParam() {}
	~RenderParam() {}

	/// @brief WVP変換行列を作成して渡す
	/// @param _transform 座標
	/// @return wvp行列
	const WVP& GetWVP(const Transform& _transform);

	/// @brief ビュー変換行列をセット
	/// @param _view ビュー変換行列
	void SetView(const DirectX::SimpleMath::Matrix& _view) { wvp.view = _view; }

	/// @brief プロジェクション変換行列をセット
	/// @param _proj プロジェクション変換行列
	void SetProjection(const DirectX::SimpleMath::Matrix& _proj) { wvp.projection = _proj; }
};

