#pragma once
#include "Collider.h"

class BoxCollider :
	public Collider
{
	DirectX::SimpleMath::Vector3 posOffset; // ゲームオブジェクトからの座標オフセット
	DirectX::SimpleMath::Vector3 angleOffset;    // 傾き
	DirectX::SimpleMath::Vector3 size;  // ボックスの大きさ

	bool isAABB = false;	// AABBなのかフラグ
public:
	using Collider::Collider;

	void Init();
	void Draw() override;

	void SetParameter() override;

	// パラメータ設定
	// 座標オフセット
	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { posOffset = _offset; } 
	void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angleOffset = _angle; }    // 角度
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { size = _size; } // 大きさ

	// あたり判定で使用する
	// ワールド座標系の各パラメータを取得する
	DirectX::SimpleMath::Vector3 GetCenterPos()const;
	DirectX::SimpleMath::Vector3 GetRotation()const;
	DirectX::SimpleMath::Vector3 GetScale()const;
};

