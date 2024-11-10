#pragma once
#include "CP_Renderer.h"
#include "Polygon2D.h"

class VertexShader;
class PixelShader;
class Texture;

class CP_UIRenderer : public CP_Renderer
{
	/// @brief UI描画に使用する頂点シェーダー
	static VertexShader* pVertexShader;

	/// @brief UI描画に使用するピクセルシェーダー
	static PixelShader* pPixelShader;

	/// @brief シェーダーが既にセットされているか？
	static bool isSetShader;

	/// @brief テクスチャを貼るポリゴン
	std::unique_ptr<Polygon2D> pPolygon;

	/// @brief 表示するテクスチャ
	const Texture* pTexture;

public:
	CP_UIRenderer();
	~CP_UIRenderer(){}

	void Init() override;
	void OnChangeScale();
	void OnChangePosition();
	void OnChangeRotation();

	/// @brief テクスチャをセット
	/// @param _setTexture セットするテクスチャ
	void SetTexture(const Texture& _setTexture);

	/// @brief UV座標をセット
	/// @param _startUV 開始UV
	/// @param _endUV 終点UV
	void SetUV(const DirectX::SimpleMath::Vector2& _startUV, const DirectX::SimpleMath::Vector2& _endUV);

	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief 描画
	void Draw() override;

	/// @brief 描画準備
	void DrawSetup();

	/// @brief サイズなど変更するためにポリゴン再生成
	void ReCreatePolygon();

	void ImGuiDebug() override;
};

