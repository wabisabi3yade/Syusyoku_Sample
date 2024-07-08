#pragma once
#include "Component.h"
// 弾の移動コンポーネント
class BulletMove :
	public Component
{
	float moveSpeed{ 0.0f };    // 移動速度
	DirectX::SimpleMath::Vector3 moveVector;    // 移動方向
	float destroyTime = 0.0f;   //発射して終わるまでの時間
	float elapsedTime = 0.0f;

public:
	using Component::Component;
	BulletMove& operator=(const BulletMove& _other);

	void Update()override;

	// 弾の移動速度・方向をセットする
	void SetMoveParameter(float _speed, DirectX::SimpleMath::Vector3 _vector, float _destroyTime);
};

