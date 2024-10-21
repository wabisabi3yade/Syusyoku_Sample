#include "pch.h"
#include "CollisionTypeJudge.h"

CollisionTypeJudge::CollisionTypeJudge()
{
}

CollisionTypeJudge::~CollisionTypeJudge()
{
}

CollisionTypeJudge::ColType CollisionTypeJudge::JudgeColKind(const CP_RigidBody& _colRbCp)
{	
	auto curItr = std::find(curColRigidBodyCp.begin(), curColRigidBodyCp.end(), &_colRbCp);
	if (curItr != curColRigidBodyCp.end())	// 既にこのフレームで衝突処理を行われているなら
		return ColType::Already;

	// 追加
	curColRigidBodyCp.emplace(&_colRbCp);

	// 前フレームでも当たっていたなら
	auto prevItr = std::find(prevColRigidBodyCp.begin(), prevColRigidBodyCp.end(), &_colRbCp);
	if (prevItr != prevColRigidBodyCp.end())	// 既にこのフレームで衝突処理を行われているなら
		return ColType::Stay;
	else
		return ColType::Enter;

	assert(!"どの条件にも達しませんでした");
}

void CollisionTypeJudge::GetExitObjectList(std::vector<const CP_RigidBody*>& _outRbComponents)
{	
	// 前は衝突していて、今回していないものを探す
	for (auto& prevObj : prevColRigidBodyCp)
	{
		auto curItr = curColRigidBodyCp.find(prevObj);
		if (curItr == curColRigidBodyCp.end())	// 現在に無かったら
		{
			// 追加する
			_outRbComponents.push_back(prevObj);
		}
	}
}

void CollisionTypeJudge::SetupForNextFrame()
{
	// 前フレームの衝突に移す
	prevColRigidBodyCp = curColRigidBodyCp;

	// 現在フレームをクリア
	curColRigidBodyCp.clear();
}
