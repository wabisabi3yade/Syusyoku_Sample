#pragma once

namespace HashiTaku
{
	/// @brief ガウシアンブラーをかける
	class GausianBlur : public IImGuiUser
	{
		struct BlurBuffer
		{
			int screenWidth = 0;
			int screenHeight = 0;
			int dummy1, dummy2;
		};

		/// @brief ブラーを掛けたレンダーターゲット
		std::unique_ptr<RenderTarget> pBlurRenderTarget;

		/// @brief バッファ
		BlurBuffer buffer;

		/// @brief 描画
		D3D11_Renderer* pRenderer;

		/// @brief	ブラーをかける頂点シェーダー
		VertexShader* pGausianVS;

		/// @brief 縦ブラーのPS
		PixelShader* pVertBlurPS;

		/// @brief 横ブラーのPS
		PixelShader* pHoriBlurPS;
	public:
		GausianBlur(u_int _width, u_int _height);
		~GausianBlur() {}

		/// @brief ブラーを適用する
		/// @param _texture ブラーをかけたいテクスチャ
		const RenderTarget& ApplyBlur(const Texture& _texture);
		
	private:
		/// @brief レンダーターゲットを生成する
		void CreateRenderTarget(u_int _width, u_int _height);

		// ImGuiデバッグ
		void ImGuiDebug() override;
	};
}


