#pragma once
#include "GameObject.h"
#include "Model.h"

// 3Dモデルを持ったオブジェクト
class Object_3D : public GameObject
{
	std::unique_ptr<Model> pModel;	// モデル情報
	const Model* pConstModel;	// 頂点座標などが変わらないならconstポインタでもらい、メモリ節約

	void Release();	// 解放処理
public:
	Object_3D();
	virtual ~Object_3D();

	void SetModel(const Model* _setModel) { pConstModel = _setModel; }

	void Update() override;	// 更新処理
	void Draw() override;	// 描画処理
};

