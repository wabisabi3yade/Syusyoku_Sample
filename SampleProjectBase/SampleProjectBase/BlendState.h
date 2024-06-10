#pragma once
#include "IGetRenderPara.h"

// 半透明処理をするためのブレンドオブジェクト
class BlendState : public IGetRenderPara<ID3D11BlendState*>
{
	std::unique_ptr<ID3D11BlendState> pBlendStateObj;

public:
	// 引数：デバイス
	BlendState(){};
	~BlendState(){};

	// 初期化
	bool Init(ID3D11Device& _pDevice);

	// ブレンドステートオブジェクトを渡す
	ID3D11BlendState* GetParaObject() override { return pBlendStateObj.get(); }
};

