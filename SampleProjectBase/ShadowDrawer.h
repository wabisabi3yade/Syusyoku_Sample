#pragma once
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IApplyDepthShadow.h"

namespace HashiTaku
{
	class DirectionLight;

	/// @brief 影描画クラス
	class ShadowDrawer : public IImGuiUser
	{
		/// @brief 深度値を書き込む描画リスト
		std::list<IApplyShadowDepth*> depthWriters;

		/// @brief 深度を書き込むときのライトからの行列
		RenderParam::WVP lightMatrix;

		/// @brief 深度を書き込むレンダーターゲット
		std::unique_ptr<RenderTarget> pRenderTarget;

		/// @brief 深度ステンシル
		std::unique_ptr<DepthStencil> pDepthStencil;

		/// @brief 深度書き込みに使用するピクセルシェーダー
		PixelShader* pDepthWritePS;

		/// @brief 描画クラス
		D3D11_Renderer* pRenderer;

		/// @brief 影を落とす光源
		const DirectionLight* pLight;
	public:
		/// @brief コンストラクタ
		/// @param _light 光源
		ShadowDrawer(const DirectionLight& _light);
		~ShadowDrawer() {}

		/// @brief 深度テクスチャを作成する
		void CreateDepthTexture();

		/// @brief 深度を書き込むオブジェクトのワールド変換行列をセット
		/// @param _worldMtx ワールド変換行列
		/// @param _pVs 描画する頂点シェーダ
		void SetWorldMatrix(const DXSimp::Matrix& _worldMtx, VertexShader* _pVs);

		/// @brief ライトからの行列を取得
		RenderParam::WVP& GetLightMatrix();

		/// @brief 深度を書き込んだレンダーターゲットビューを取得
		/// @return 深度を書き込んだレンダーターゲットビュー
		RenderTarget& GetDepthRTV();

		/// @brief 深度書き込みを行うクラスを追加
		/// @param _pRenderer 書き込みを行うクラス変数
		void AddDepthWriteRenderer(IApplyShadowDepth& _renderer);

		/// @brief 深度書き込みを行うクラスを削除
		/// @param _pRenderer 書き込みを行うクラス変数
		void RemoveDepthWriteRenderer(IApplyShadowDepth& _renderer);

	private:
		/// @brief 深度書き込みできるか？
		/// @return 深度書き込みできるか？
		bool GetCanDepthWrite() const;

		/// @brief ライトから見た行列を計算する
		void CalcLightMatrix();

		void ImGuiDebug() override;
	};
}