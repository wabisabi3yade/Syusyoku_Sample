#pragma once
#include "ISetUpProjection.h"
#include "IGetRenderResource.h"

namespace HashiTaku
{
	// プロジェクション変換行列を作成する（透視投影をする）クラス
	class SetUpPerspectiveProj : public ISetUpProjection
	{
		float fov;	// 視野角
		float nearZ;	// 描画距離（最短）
		float farZ;	// 描画距離(最長)


	public:
		SetUpPerspectiveProj();
		~SetUpPerspectiveProj();

		/// <summary>
		/// ビュー変換行列を作成する
		/// </summary>
		/// <param name="_screenWidth"></param>
		/// <param name="_screenHeight"></param>
		/// <returns></returns>
		bool SetUpProjection(UINT _screenWidth, UINT _screenHeight, IGetRenderResource* _pRenderResource) override;
	};
}