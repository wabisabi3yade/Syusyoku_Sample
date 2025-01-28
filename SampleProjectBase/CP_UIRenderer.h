#pragma once
#include "CP_Renderer.h"
#include "Polygon2D.h"

namespace HashiTaku
{
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

		/// @brief 表示するテクスチャ
		const Texture* pTexture;

		/// @brief α値
		float alpha;

	protected:
		/// @brief テクスチャを貼るポリゴン
		std::unique_ptr<Polygon2D> pPolygon;

	public:
		CP_UIRenderer();
		virtual ~CP_UIRenderer() {}

		void Init() override;
		void OnChangeScale();
		void OnChangePosition();
		void OnChangeRotation();

		/// @brief テクスチャをセット
		/// @param _pSetTexture セットするテクスチャ
		void SetTexture(const Texture* _pSetTexture);

		/// @brief UV座標をセット
		/// @param _startUV 開始UV
		/// @param _endUV 終点UV
		void SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV);

		/// @brief α値をセット
		/// @param _alpha α値
		void SetAlpha(float _alpha);

		/// @brief α値をセット
		/// @return α値
		float GetAlpha() const;

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;

	protected:
		/// @brief サイズなど変更するためにポリゴン再生成(トランスフォームから自動で)
		virtual void ReCreatePolygon();

		void ImGuiDebug() override;
	private:
		/// @brief 描画
		void Draw() override;

		/// @brief 描画準備
		void DrawSetup();
	};
}