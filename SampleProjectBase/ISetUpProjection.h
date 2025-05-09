#pragma once

namespace HashiTaku
{
	class IGetRenderResource;

	// 投影方法の変換行列を求めるインターフェース
	class ISetUpProjection
	{
	public:
		// 投影行列を求めて、返す処理
		virtual bool SetUpProjection(UINT screenWidth, UINT screenHeight, IGetRenderResource* _pRenderResource) = 0;
	};
}