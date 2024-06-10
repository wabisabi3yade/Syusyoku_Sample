#pragma once
#include "GameObject.h"
#include "Model.h"

// 3Dモデルを持ったオブジェクト
class Object_3D : public GameObject
{
	//std::unique_ptr<Model> pModel;	// モデル情報
	Model* pModel;

	void Release();	// 解放処理
public:
	Object_3D();
	virtual ~Object_3D();

	void SetModel(Model* _setModel) { pModel = _setModel; }

	void Update() override;	// 更新処理
	void Draw() override;	// 描画処理

	Model& GetModel() { return *pModel; }
};

