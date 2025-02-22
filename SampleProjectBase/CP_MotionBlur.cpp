#include "pch.h"
#include "CP_MotionBlur.h"
#include "GameObject.h"

namespace HashiTaku
{
	CP_MotionBlur::CP_MotionBlur() :
		CP_PostEffect(CP_PostEffect::PostEffectType::Blur),
		pMeshRenderer(nullptr),
		isDrawingBlur(false)
	{
	}

	void CP_MotionBlur::Start()
	{
		pMeshRenderer = GetGameObject().GetComponent<CP_MeshRenderer>();

		// 活動状態をオフ
		if (!pMeshRenderer)
			SetEnable(false);
	}

	void CP_MotionBlur::Draw()
	{
		// 現在フレームのメッシュ描画を行う
		const RenderParam::WVP& curWVP = pMeshRenderer->GetDrawMatrix();

		// ブラーを描画する
		if (isDrawingBlur)
		{
			DrawBlur(curWVP);
		}

		// 次フレームの為に更新
		prevDrawMeshWVP = curWVP;
	}

	void CP_MotionBlur::SetDrawBlur(bool _drawBlur)
	{
		isDrawingBlur = _drawBlur;
	}

	void CP_MotionBlur::DrawBlur(const RenderParam::WVP& _curDrawMeshWVP)
	{

	}
}
