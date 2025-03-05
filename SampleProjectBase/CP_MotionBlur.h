#pragma once
#include "Component.h"
#include "CP_MeshRenderer.h"

namespace HashiTaku
{
	class CP_MeshRenderer;

	/// @brief モーションブラーをかけるコンポーネント
	class CP_MotionBlur : public CP_Renderer
	{
		/// @brief 1フレーム前の描画
		RenderParam::WVP prevDrawMeshWVP;

		/// @brief メッシュ描画クラス
		CP_MeshRenderer* pMeshRenderer;

		/// @brief レンダーターゲット管理
		RenderTargetCollection* pRTCollection;

		/// @brief 書き込む頂点シェーダー
		VertexShader* pWriteVS;

		/// @brief 書き込むピクセルシェーダー
		PixelShader* pWritePS;

		/// @brief ブラーを描画する
		bool isDrawingBlur;
	public:
		CP_MotionBlur();
		~CP_MotionBlur() {}

		/// @brief ブラーを描画
		/// @param _drawBlur ブラー描画
		void SetDrawBlur(bool _drawBlur);

	private:
		/// @brief 初期化
		void Init();

		/// @brief 開始
		void Start() override;

		/// @brief 描画
		void Draw() override;

		/// @brief 描画準備
		void DrawSetup(const RenderParam::WVP& _curDrawMeshWVP);

		/// @brief レンダーターゲットにブラーのベクトルを書く
		/// @param _curDrawMeshWVP 現在のメッシュのWVP
		void DrawBlurTexture(const RenderParam::WVP& _curDrawMeshWVP);

		void ImGuiDebug() override;
	};
}


