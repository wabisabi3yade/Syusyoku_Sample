#include "pch.h"
#include "ShadowDrawer.h"
#include "DirectionLight.h"
#include "ShaderCollection.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// 深度マップのテクスチャサイズ
	constexpr u_int TEXTURE_WIDTH(1920);
	constexpr u_int TEXTURE_HEIGHT(1080);

	// ビューポートサイズ
	constexpr u_int RENDERTARGET_WIDTH(1920);
	constexpr u_int RENDERTARGET_HEIGHT(1080);

	// 深度書き込みに使用するシェーダー名
	constexpr auto DEPTHWRITE_PSNAME("PS_DepthWrite");

	// ディレクションライトの座標を求めるときの注視点からカメラからの距離
	constexpr float CAM_TARGET_DISTANCE(25.0f);

	// ディレクションライトの座標をもとめるときのカメラの注視点からの距離
	constexpr float DIR_LIGHT_DISTACE(50.0f);

	ShadowDrawer::ShadowDrawer(const DirectionLight& _light) :
		pDepthWritePS(nullptr), pLight(nullptr)
	{
		// ライト取得
		pLight = &_light;

		// 深度書き込みテクスチャ
		pDepthStencil = std::make_unique<DepthStencil>();
		pDepthStencil->Create(TEXTURE_WIDTH, TEXTURE_HEIGHT, false);

		// 深度描画レンダーターゲット
		pRenderTarget = std::make_unique<RenderTarget>();
		pRenderer = Direct3D11::GetInstance()->GetRenderer();
		pRenderTarget->Create(DXGI_FORMAT_R32G32B32A32_FLOAT,
			RENDERTARGET_WIDTH,
			RENDERTARGET_HEIGHT);

		// シェーダー取得
		auto* pShCol = ShaderCollection::GetInstance();

		pDepthWritePS = pShCol->GetPixelShader(DEPTHWRITE_PSNAME);
		if (!pDepthWritePS) HASHI_DEBUG_LOG("ピクセルシェーダー取得失敗");
	}

	void ShadowDrawer::CreateDepthTexture()
	{
#ifdef EDIT
		if (!GetCanDepthWrite()) return;
#endif // EDIT

		// レンダーターゲットと深度書き込みテクスチャをクリア
		pRenderTarget->Clear();
		pDepthStencil->ClearColor();

		// ライトから見た行列を作成する
		CalcLightMatrix();

		// レンダーターゲット切り替える(頂点シェーダーはそれぞれの描画で使用しているものを使用)
		pRenderer->SetRenderTerget(1, pRenderTarget.get(), pDepthStencil.get());
		pDepthWritePS->SetGPU();

		// 深度書き込んでいく
		for (auto& write : depthWriters)
		{
			write->WriteDepth();
		}

		// 終わったらレンダーターゲットを元に戻す
		pRenderer->SetBaseRenderTarget();
	}

	void ShadowDrawer::CalcLightMatrix()
	{
		auto* pInSceneManager = InSceneSystemManager::GetInstance();
		if (pInSceneManager->GetIsNullCamera()) return;	// カメラがセットされていないなら

		// ディレクションライトの座標をカメラから求める
		Transform& camTrans = pInSceneManager->GetMainCamera().GetTransform();
		DXSimp::Vector3 camTargetPos = camTrans.GetPosition() +
			camTrans.Forward() * CAM_TARGET_DISTANCE;

		// ディレクションライトの座標を求める
		DXSimp::Vector3 lightDirection = pLight->GetDirection();
		lightDirection.Normalize();
		DXSimp::Vector3 lightPos = camTargetPos - lightDirection * DIR_LIGHT_DISTACE;
		// ライトから見た行列を作成する
		DXSimp::Matrix lightViewMtx = DirectX::XMMatrixLookAtLH(
			lightPos,
			lightPos + lightDirection,
			Vec3::Up
		);
		lightViewMtx.Transpose(lightMatrix.view);

		// プロジェクション
		DXSimp::Matrix projtMtx = DirectX::XMMatrixOrthographicLH(
			50.0f,
			50.0f,
			0.1f,
			100.0f
		);

		projtMtx.Transpose(lightMatrix.projection);
	}

	bool ShadowDrawer::GetCanDepthWrite() const
	{
		if (!pDepthWritePS) return false;
		if (!pLight) return false;

		return true;
	}

	void ShadowDrawer::SetWorldMatrix(const DXSimp::Matrix& _worldMtx, VertexShader* _pVs)
	{
		lightMatrix.world = _worldMtx;

		_pVs->UpdateSubResource(0, &lightMatrix);
		_pVs->SetGPU();
	}

	RenderParam::WVP& ShadowDrawer::GetLightMatrix()
	{
		return lightMatrix;
	}

	RenderTarget& ShadowDrawer::GetDepthRTV()
	{
		return *pRenderTarget;
	}

	void ShadowDrawer::AddDepthWriteRenderer(IApplyShadowDepth& _renderer)
	{
		auto itr = std::find(depthWriters.begin(), depthWriters.end(), &_renderer);
		if (itr != depthWriters.end())
		{
			HASHI_DEBUG_LOG("既に追加されています");
			return;
		}

		depthWriters.push_back(&_renderer);
	}

	void ShadowDrawer::RemoveDepthWriteRenderer(IApplyShadowDepth& _renderer)
	{
		depthWriters.remove(&_renderer);
	}

	void ShadowDrawer::ImGuiDebug()
	{
#ifdef EDIT
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)&pDepthStencil->GetSRV(), viewportSize);
#endif // EDIT
	}
}