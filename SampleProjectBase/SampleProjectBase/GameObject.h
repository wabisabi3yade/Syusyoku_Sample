#pragma once
#include "Model.h"

// 画面に写されるオブジェクトクラス
class GameObject
{
	std::unique_ptr<Model> pModel;	// モデル情報
	const Model* pConstModel;	// 頂点座標などが変わらないならconstポインタでもらい、メモリ節約

	void Release();	// 解放処理
public:
	Transform transform;	// 移動情報

	GameObject();
	virtual ~GameObject();

	void SetModel(const Model* _setModel) { pConstModel = _setModel; }

	void Update();	// 更新処理
	void Draw();	// 描画処理
};

