#pragma once
#include "IGetRenderPara.h"

// 半透明処理をするためのブレンドオブジェクト
class BlendState : public IGetRenderPara<ID3D11BlendState*>
{
	ID3D11BlendState* pBlendStateObj{ nullptr };

	// 初期化
	bool BlendStateInit(ID3D11Device* _pDevice);
public:
	// 引数：デバイス
	BlendState(ID3D11Device* _pDevice);
	~BlendState();

	// ブレンドステートオブジェクトを渡す
	ID3D11BlendState* GetParaObject() override { return pBlendStateObj; }
};

