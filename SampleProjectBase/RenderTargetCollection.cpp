#include "pch.h"
#include "RenderTargetCollection.h"

namespace HashiTaku
{
	void RenderTargetCollection::SetRenderTarget(RenderTargetType _setRTType)
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

		// レンダーターゲットを描画先にセット
		pRenderer->SetRenderTerget(1, pRenderTarget, nullptr);
	}
}

