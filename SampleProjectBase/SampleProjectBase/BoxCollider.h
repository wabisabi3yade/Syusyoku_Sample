#pragma once
#include "Collider.h"

class BoxCollider :
	public /*Collider*/ Component
{
	DirectX::SimpleMath::Vector3 offset; // ゲームオブジェクトからの座標オフセット
	DirectX::SimpleMath::Vector3 angle;    // 傾き
	DirectX::SimpleMath::Vector3 size;  // ボックスの大きさ

	bool isHit = false;	// 消す
public:
	using Component::Component;

	void Init();
	void Update() override;
	void Draw() override;

	void SetParameter() override;

	// パラメータ設定
	// 座標オフセット
	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { offset = _offset; } 
	void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angle = _angle; }    // 角度
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { size = _size; } // 大きさ
};

