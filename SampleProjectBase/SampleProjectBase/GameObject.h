#pragma once
#include "Model.h"

// 画面に写されるオブジェクトクラス
class GameObject
{
	const Model* model;	// モデル情報

	void Release();	// 解放処理
public:
	Transform transform;	// 移動情報

	GameObject();
	virtual ~GameObject();

	void SetModel(const Model* _setModel) { model = _setModel; }

	void Update();	// 更新処理
	void Draw();	// 描画処理
};

