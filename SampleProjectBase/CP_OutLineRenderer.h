#pragma once
#include "CP_Renderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace HashiTaku
{
	class Mesh_Group;
	class IBoneBufferSupplier;

	/// @brief アウトライン描画コンポーネント(MeshRendererの後にする)
	class CP_OutLineRenderer : public CP_Renderer
	{
		struct OutLineParameter
		{
			/// @brief 線の色
			DXSimp::Color lineColor;

			/// @brief 線の太さ
			float lineScale{ 0.07f };
		};

		/// @brief アウトラインのパラメータ
		OutLineParameter lineParameter;

		/// @brief アウトライン頂点シェーダー
		static VertexShader* pOutLineVS;

		/// @brief アニメーションを反映させる頂点シェーダー
		static VertexShader* pAnimationOutLineVS;

		/// @brief アウトラインピクセルシェーダー
		static PixelShader* pOutLinePS;

		/// @brief 使用している頂点シェーダー
		VertexShader* pUseVetrexShader;

		/// @brief ボーンのバッファ供給クラス
		IBoneBufferSupplier* pBoneBuffer;

		/// @brief 描画するメッシュ
		Mesh_Group* pRenderMesh;
	public:
		CP_OutLineRenderer();
		~CP_OutLineRenderer() {}

		/// @brief 初期化
		void Init() override;

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;

	private:
		void Start() override;

		/// @brief 描画
		void Draw() override;
		void DrawSetup();

		/// @brief  アウトラインシェーダーを取得する
		void GetShader();

		/// @brief MeshRendererからメッシュを取得する
		void GetRenderMesh();

		/// @brief 使用する頂点シェーダーをセットする
		void SetUseShader();

		/// @brief アニメーションしているか取得
		/// @return アニメーションしているか？
		bool IsAnimation() const;

		/// @brief ロード行列作成
		/// @return ロード行列
		DXSimp::Matrix MakeLoadMatrix();

		void ImGuiDebug() override;
	};
}