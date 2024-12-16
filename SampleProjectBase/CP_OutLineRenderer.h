#pragma once
#include "CP_Renderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace HashiTaku
{
	class Mesh_Group;

	/// @brief アウトライン描画コンポーネント(MeshRendererの後にする)
	class CP_OutLineRenderer : public CP_Renderer
	{
		/// @brief アウトライン頂点シェーダー
		static VertexShader* pOutLineVS;

		/// @brief アウトラインピクセルシェーダー
		static PixelShader* pOutLinePS;

		/// @brief 描画するメッシュ
		Mesh_Group* pRenderMesh;
	public:
		CP_OutLineRenderer();
		~CP_OutLineRenderer() {}

		/// @brief 初期化
		void Init() override;
	private:
		void Start() override;

		/// @brief 描画
		void Draw() override;
		void DrawSetup();

		/// @brief  アウトラインシェーダーを取得する
		void GetShader();

		/// @brief MeshRendererからメッシュを取得する
		void GetRenderMesh();

		/// @brief ロード行列作成
		/// @return ロード行列
		DXSimp::Matrix MakeLoadMatrix();

		void ImGuiDebug() override;
	};
}