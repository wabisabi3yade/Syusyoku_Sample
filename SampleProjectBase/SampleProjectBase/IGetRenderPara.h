#pragma once


template<typename T>
// 描画のパラメータオブジェクトのインターフェース
class IGetRenderPara
{
public:
	virtual T GetParaObject() = 0;
};

