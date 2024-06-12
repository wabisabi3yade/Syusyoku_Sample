#pragma once
#include "Mesh.h"
#include "Material.h"

// キューブ、球など基本的なメッシュクラスの基底
class BasicObject_Base : public Mesh
{
protected:
	Transform transform;
	const Material* material;	// マテリアル

	// 基本オブジェクトはマテリアルでなくこれで色を決める
	DirectX::SimpleMath::Color color;	

	virtual void Make() = 0;	// 各メッシュを作成
	
public:
	BasicObject_Base();
	virtual ~BasicObject_Base();

	virtual void Draw();	// 描画

	// パラメータ代入
	void SetPosition(const DirectX::SimpleMath::Vector3& _position) { transform.position = _position; }
	void SetRotation(const DirectX::SimpleMath::Vector3& _rotation) { transform.position = _rotation; }
	void SetScale(const DirectX::SimpleMath::Vector3& _scale) { transform.position = _scale; }
	void SetTransform(const Transform& _transform) { transform = _transform; }
};