#pragma once
#include "Component.h"
#include "Bullet.h"

// 戦車の弾発射クラス
class PlayerShot : public Component
{
	// 弾の移動速度
	float shotSpeed;	

	// 弾の飛ばす方向
	DirectX::SimpleMath::Vector3 shotVec;		

	// 弾プレハブ
	std::unique_ptr<Bullet> bullet;

	// 中心からの発射場所距離
	float shotDistance = 0.0f;

public:
	using Component::Component;

	void Init() override;
	void Shot();	// 弾を発射する
};

