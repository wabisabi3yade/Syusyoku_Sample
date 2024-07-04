#pragma once
#include "Mesh.h"
#include "MaterialClass.h"

// キューブ、球など基本的なメッシュクラスの基底
class BasicObject_Base : public Mesh
{
protected:
	// Geomwtoryから描画処理でのマテリアル
	Material* pBaseMaterial;

	// マテリアル
	Material* pMaterial;	

	virtual void Make() = 0;	// 各メッシュを作成
public:
	BasicObject_Base();
	virtual ~BasicObject_Base();

	virtual void BasicDraw(Transform& _transform, DirectX::SimpleMath::Color& _color);	// 描画
	void SetMaterial(Material& _material);

	void ImGuiSetting();
};