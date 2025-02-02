#pragma once
#include "CP_Renderer.h"
#include "IApplyDepthShadow.h"
#include "IBoneSupplier.h"

// 描画に必要
#include "Mesh_Group.h"

namespace HashiTaku
{
	class ShadowDrawer;

	// メッシュ描画コンポーネント
	class CP_MeshRenderer : public CP_Renderer, public IApplyShadowDepth
	{
		/// @brief 描画するメッシュ群
		Mesh_Group* pRenderMesh{ nullptr };

		/// @brief 使用するマテリアル（ないときはメッシュのマテリアルを使用する）
		std::vector<Material*> setMaterials;

		/// @brief メッシュの位置オフセット
		DXSimp::Vector3 offsetMeshPosition;

		/// @brief 影を描画するクラス
		ShadowDrawer* pShadowDrawer;

		/// @brief ボーンバッファを供給してくれるクラス
		IBoneBufferSupplier* pBoneBuffer;

		/// @brief オブジェクトの原点を表示するか
		bool isOriginDisplay;

		/// @brief 影を描画させるか？
		bool isShadow;
	public:
		CP_MeshRenderer();
		~CP_MeshRenderer() {}

		/// @brief 初期化
		void Init() override;

		/// @brief 削除時処理
		void OnDestroy() override;

		/// @brief 描画するメッシュをセット
		/// @param _renderMesh メッシュ群
		void SetRenderMesh(Mesh_Group& _renderMesh);

		/// @brief 描画するメッシュをセット
		/// @param _assetName メッシュ名
		void SetRenderMesh(const std::string& _assetName);

		// シェーダーの名前からシェーダーを変更する
		void SetVertexShader(const std::string& _vsName);
		void SetPixelShader(const std::string& _psName);

		/// @brief メッシュのオフセットをセット
		/// @param _offsetPos セットするオフセット座標
		void SetMeshOffset(const DXSimp::Vector3& _offsetPos);

		/// @brief 描画するメッシュを取得
		/// @return メッシュ群
		Mesh_Group* GetRenderMesh();
		
		/// @brief メッシュの座標オフセットを取得
		/// @return メッシュの座標オフセット
		const DXSimp::Vector3& GetMeshOffsetPos() const;

		/// @brief 深度書き込みを行う
		void WriteDepth() override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		// コンポーネント共通関数
		void Draw() override;

		/// @brief 描画できるのか返す
		/// @return 描画できるか？
		bool IsCanDraw();

		/// @brief ロード時のオフセット行列計算
		/// @return ロード行列
		DXSimp::Matrix CalcLoadMtx();

		/// @brief メッシュを描画
		void DrawMesh(RenderParam::WVP& _wvp);

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
		DXSimp::Vector3 WorldOffset(const DXSimp::Vector3& _offset);

		/// @brief 原点表示
		void OriginDisplay();

		/// @brief マテリアルを取得する
		/// @param meshIdx メッシュID
		Material* GetMaterial(u_int meshIdx);

		void ImGuiDebug() override;
	};
}