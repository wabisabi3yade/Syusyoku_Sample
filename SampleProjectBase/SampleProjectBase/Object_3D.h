#pragma once
#include "GameObject.h"
#include "Model.h"

// 3Dモデルを持ったオブジェクト
class Object_3D : public GameObject
{
	Model* pModel;	// モデル

	void Release();	// 解放処理
public:
	Object_3D();
	virtual ~Object_3D();

	void Update() override;

	void Draw() override;	// 描画処理

	void SetModel(Model* _setModel) { pModel = _setModel; }

	Model& GetModel() { return *pModel; }
};

