#pragma once
#include "CP_Renderer.h"

// 描画に必要
#include "Mesh_Group.h"

namespace HashiTaku
{
	// メッシュ描画コンポーネント
	class CP_MeshRenderer : public CP_Renderer
	{
		/// @brief 描画するメッシュ群
		Mesh_Group* pRenderMesh{ nullptr };

		/// @brief 使用するマテリアル（ないときはメッシュのマテリアルを使用する）
		std::vector<Material*> setMaterials;

		/// @brief オブジェクトの原点を表示するか
		bool isOriginDisplay;
	public:
		CP_MeshRenderer();
		~CP_MeshRenderer() {}

		/// @brief 描画するメッシュをセット
		/// @param _renderMesh メッシュ群
		void SetRenderMesh(Mesh_Group& _renderMesh);

		/// @brief 描画するメッシュをセット
		/// @param _assetName メッシュ名
		void SetRenderMesh(const std::string& _assetName);

		// シェーダーの名前からシェーダーを変更する
		void SetVertexShader(const std::string& _vsName);
		void SetPixelShader(const std::string& _psName);

		/// @brief 描画するメッシュを取得
		/// @return メッシュ群
		Mesh_Group* GetRenderMesh();

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		// コンポーネント共通関数
		void Start() override;
		void Draw() override;

		/// @brief 描画できるのか返す
		/// @return 描画できるか？
		bool IsCanDraw();

		/// @brief ロード時のオフセット行列計算
		/// @return ロード行列
		DirectX::SimpleMath::Matrix CalcLoadMtx();

		/// @brief メッシュを描画
		void DrawMesh(RenderParam::WVP& _wvp);

		/// @brief 影描画
		void DrawShadow();

		/// @brief マテリアルの準備
		/// @param _wvp wvp行列
		/// @param _pMaterial　マテリアル
		void MaterialSetup(RenderParam::WVP& _wvp, Material* _pMaterial);

		/// @brief シェーダーの準備
		/// @param _shader シェーダー
		/// @param _wvp wvp行列
		/// @param _material マテリアル
		void ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp, Material& _material);

		/// @brief ローカル軸を考慮したオフセットに変換
		/// @param _worldOffset 変換したいオフセット
		DirectX::SimpleMath::Vector3 WorldOffset(const DirectX::SimpleMath::Vector3& _offset);

		/// @brief 原点表示
		void OriginDisplay();

		void ImGuiDebug() override;
	};
}