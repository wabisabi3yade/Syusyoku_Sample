#pragma once
#include "Mesh.h"
#include "MaterialClass.h"

// キューブ、球など基本的なメッシュクラスの基底
class BasicObject_Base : public Mesh
{
protected:
	MaterialClass* pMaterial;	// マテリアル
	virtual void Make() = 0;	// 各メッシュを作成
	
public:
	BasicObject_Base();
	virtual ~BasicObject_Base();

	virtual void Draw(Transform& _transform, DirectX::SimpleMath::Color& _color);	// 描画
	void SetMaterial(MaterialClass& _material);
};