#pragma once

template<typename T>
// 描画のパラメータオブジェクトのインターフェース
class IGetRenderPara
{
public:
	virtual T GetResourceObject() = 0;
};

