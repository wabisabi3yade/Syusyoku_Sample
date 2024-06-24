#include "pch.h"
#include "Collider.h"
#include "CollisionRegister.h"
using namespace DirectX::SimpleMath;

const Color Collider::normalColor = { 0,1,0,1 };
const Color Collider::hitColor = { 1,0,0,1 };

Collider::~Collider()
{
	// 当たり判定チェッカーから削除する
	CollisionRegister::GetInstance()->PopCollider(*this);
}

void Collider::Init()
{
	// シーン内の当たり判定チェッカーに追加する
	CollisionRegister::GetInstance()->AddCollider(*this);
}
