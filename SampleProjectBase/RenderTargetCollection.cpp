#include "pch.h"
#include "RenderTargetCollection.h"

namespace HashiTaku
{
	RenderTargetCollection::RenderTargetCollection(D3D11_Renderer& renderer) :
		pRenderer(&renderer)
	{		
		// レンダーターゲットを作成
		CreateRenderTargets();
	}

	void RenderTargetCollection::CreateRenderTargets()
	{
		const u_int renderTargetNum = static_cast<u_int>(RenderTargetType::MaxNum);

		u_int screenWidth = pRenderer->GetWindowWidth();
		u_int screenHeight = pRenderer->GetWindowHeight();

		struct RenderTargetInfo
		{
			DXSimp::Color clearColor;	// クリア時の色
			DXSimp::Vector2 sizeTimes;	// サイズ（スクリーンサイズの倍率）
			DXGI_FORMAT format;	// フォーマット
		};

		// 各レンダーターゲットの情報を設定
		// サイズ情報リスト
		std::array<RenderTargetInfo, renderTargetNum> rtCreateParam;
		{
			using enum RenderTargetType;

			// シーン描画
			rtCreateParam[static_cast<u_int>(SceneDraw)] =
			{ DXSimp::Vector4::One , 
				DXSimp::Vector2(1.0f, 1.0f),  
				DXGI_FORMAT_R8G8B8A8_UNORM };

			// ブラー
			rtCreateParam[static_cast<u_int>(Blur)] =
			{ DXSimp::Color(0.0f, 0.0f, 0.0f, 1.0f),
				DXSimp::Vector2(0.5f, 0.5f),
				DXGI_FORMAT_R16G16B16A16_FLOAT};

			// ブルーム
			rtCreateParam[static_cast<u_int>(Bloom)] =
			{ DXSimp::Color(0.0f, 0.0f, 0.0f, 1.0f),
				DXSimp::Vector2(0.5f, 0.5f),
				DXGI_FORMAT_R16G16B16A16_FLOAT };
		}

		// 各レンダーターゲット作成
		for (u_int r_i = 0; r_i < renderTargetNum; r_i++)
		{
			std::unique_ptr<RenderTarget> pCreateRT = std::make_unique<RenderTarget>();
			pCreateRT->Create(rtCreateParam[r_i].format,
				static_cast<u_int>(screenWidth * rtCreateParam[r_i].sizeTimes.x),
				static_cast<u_int>(screenHeight * rtCreateParam[r_i].sizeTimes.y),
				rtCreateParam[r_i].clearColor);

			// 追加
			renderTargetList[r_i] = std::move(pCreateRT);
		}
	}

	void RenderTargetCollection::SetRenderTarget(RenderTargetType _setRTType,
		bool _useDepthStencil)
	{
		RenderTarget* pRenderTarget = renderTargetList[static_cast<u_int>(_setRTType)].get();

#ifdef _DEBUG
		if (!pRenderTarget)	// レンダーターゲットがなければ
		{
			// エラーメッセージを出す
			std::string message = std::string(magic_enum::enum_name(_setRTType));
			message += "のレンダーターゲットがありません";
			assert(!message.c_str());

			return;
		}
#endif // _DEBUG

		// Zバッファ書き込むか
		ID3D11DepthStencilView* pStencilView = nullptr;
		if (_useDepthStencil)
			pStencilView = pRenderer->GetDepthStencil();

		// レンダーターゲットを描画先にセット
		pRenderer->SetRenderTerget(1, 
			pRenderTarget,
			pStencilView);
	}

	RenderTarget* RenderTargetCollection::GetRenderTarget(RenderTargetType _setRTType)
	{
#ifdef _DEBUG
		if (!renderTargetList[static_cast<u_int>(_setRTType)])	// レンダーターゲットがなければ
		{
			// エラーメッセージを出す
			std::string message = std::string(magic_enum::enum_name(_setRTType));
			message += "のレンダーターゲットがありません";
			assert(!message.c_str());

			return nullptr;
		}
#endif // _DEBUG

		return renderTargetList[static_cast<u_int>(_setRTType)].get();
	}

	void RenderTargetCollection::Clear()
	{
		// 全てクリアする
		for (auto& pRT : renderTargetList)
		{
			pRT->Clear();
		}
	}
}

