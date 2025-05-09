#pragma once
#include "CP_Renderer.h"

// 描画関係
#include "Sprite.h"
#include "Material.h"

namespace HashiTaku
{
	// スプライトを描画するコンポーネント
	class CP_SpriteRenderer : public CP_Renderer
	{
		// スプライト
		std::unique_ptr<Sprite> pSprite;

		/// @brief α値
		float alpha;

		// マテリアル
		Material* pMaterial{ nullptr };

		/// @brief 描画する頂点シェーダー
		VertexShader* pDrawVS;

		/// @brief 描画するピクセルシェーダー
		PixelShader* pDrawPS;

		// マテリアルの準備
		void MaterialSetup();

		// 描画準備
		void DrawSetup();

	public:
		CP_SpriteRenderer();
		~CP_SpriteRenderer(){}

		void Init() override;

		void Draw() override;

		void SetTexture(Texture& _texture);

		void SetMaterial(Material& _material);

		/// @brief α値をセット
		/// @param _alpha α値
		void SetAlpha(float _alpha);

		/// @brief α値を取得する
		/// @return α値
		float GetAlpha() const;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void ImGuiDebug() override;

		// 使用するシェーダーをImGuiで
		void ImGuiUseShader();
	};
}