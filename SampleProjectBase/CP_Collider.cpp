#include "pch.h"
#include "CP_Collider.h"
#include "Tag.h"

#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

const Color CP_Collider::normalColor = { 1,1,1,0.8f };
const Color CP_Collider::hitColor = { 1,0,0,1 };

CP_Collider::CP_Collider(Type _type) : type(_type)
{
}

CP_Collider::~CP_Collider()
{
	// 当たり判定チェッカーから削除する
	CollisionChecker& colCheck = InSceneSystemManager::GetInstance()->GetCollisonChecker();
	colCheck.PopCollider(*this);
}

void CP_Collider::Start()
{
	// シーン内の当たり判定チェッカーに追加する
	CollisionChecker& colCheck = InSceneSystemManager::GetInstance()->GetCollisonChecker();
	colCheck.AddCollider(*this);
}

void CP_Collider::SetTagColor(const Tag& _tag)
{
#ifdef EDIT







#endif // EDIT
}

void CP_Collider::SetLayerColor(const Layer& _layer)
{
#ifdef EDIT
	






#endif // EDIT
}
