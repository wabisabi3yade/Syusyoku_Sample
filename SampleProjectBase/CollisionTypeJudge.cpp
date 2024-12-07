#include "pch.h"
#include "CollisionTypeJudge.h"
#include "CP_RigidBody.h"

CollisionTypeJudge::CollisionTypeJudge(CP_RigidBody& _rb)
{
	pMyRb = &_rb;
}

CollisionTypeJudge::~CollisionTypeJudge()
{
}

CollisionTypeJudge::ColType CollisionTypeJudge::JudgeColKind(CP_RigidBody& _colRbCp)
{
	auto curItr = std::find(curColRigidBodyCp.begin(), curColRigidBodyCp.end(), &_colRbCp);
	if (curItr != curColRigidBodyCp.end())	// ���ɂ��̃t���[���ŏՓˏ������s���Ă���Ȃ�
		return ColType::Already;

	// �����Ƒ���ɒǉ�
	curColRigidBodyCp.emplace(&_colRbCp);
	_colRbCp.AddCurrentCollision(*pMyRb);

	// �O�t���[���ł��������Ă����Ȃ�
	auto prevItr = std::find(prevColRigidBodyCp.begin(), prevColRigidBodyCp.end(), &_colRbCp);
	if (prevItr != prevColRigidBodyCp.end())	// ���ɂ��̃t���[���ŏՓˏ������s���Ă���Ȃ�
		return ColType::Stay;
	else
		return ColType::Enter;

	assert(!"�ǂ̏����ɂ��B���܂���ł���");
}

void CollisionTypeJudge::GetExitObjectList(std::vector<const CP_RigidBody*>& _outRbComponents)
{	
	// �O�͏Փ˂��Ă��āA���񂵂Ă��Ȃ����̂�T��
	for (auto& prevObj : prevColRigidBodyCp)
	{
		auto curItr = curColRigidBodyCp.find(prevObj);
		if (curItr == curColRigidBodyCp.end())	// ���݂ɖ���������
		{
			// �ǉ�����
			_outRbComponents.push_back(prevObj);
		}
	}
}

void CollisionTypeJudge::AddCurrentColRb(CP_RigidBody& _colRbCp)
{
	curColRigidBodyCp.emplace(&_colRbCp);
}

void CollisionTypeJudge::SetupForNextFrame()
{
	// �O�t���[���̏Փ˂Ɉڂ�
	prevColRigidBodyCp = curColRigidBodyCp;

	// ���݃t���[�����N���A
	curColRigidBodyCp.clear();
}
