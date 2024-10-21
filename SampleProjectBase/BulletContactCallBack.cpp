#include "pch.h"
#include "BulletContactCallBack.h"
#include "GameObject.h"
#include "CollisionInfo.h"

btScalar BulletContactCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	using namespace HashiTaku::Bullet;

	// �Փ˃I�u�W�F�N�g���擾
	const btCollisionObject& objA = *colObj0Wrap->getCollisionObject();
	const btCollisionObject& objB = *colObj1Wrap->getCollisionObject();

	// �Փ˂����ǂ��炩�������ȉ��̏������s���΂����̂ŃA�h���X���傫��������s���悤�ɐ�������
	if (&objA < &objB) return 0;

	// �Փˏ����擾
	CollisionInfo infoA;
	infoA.pRigidBodyCp = static_cast<CP_RigidBody*>(objA.getUserPointer());
	infoA.contactPoint = ToDXVector3(cp.getPositionWorldOnA());
	CollisionInfo infoB;
	infoB.pRigidBodyCp = static_cast<CP_RigidBody*>(objB.getUserPointer());
	infoB.contactPoint = ToDXVector3(cp.getPositionWorldOnB());

	// �Փ˃^�C�v�𔻒肵�Ă��ꂼ��̏������s�킹��
	GameObject& gameObjA = infoA.pRigidBodyCp->GetGameObject();
	GameObject& gameObjB = infoB.pRigidBodyCp->GetGameObject();
	CollisionTypeJudge::ColType colType = infoA.pRigidBodyCp->GetColTypeJudge().JudgeColKind(*infoB.pRigidBodyCp);

	// �Փ˃^�C�v�ɂ�鏈��
	switch (colType)
	{
	case CollisionTypeJudge::ColType::Already: // ���̃t���[����2��ڂ̏����Ȃ� 
		break;

	case CollisionTypeJudge::ColType::Enter: // �ՓˊJ�n���Ȃ�
		gameObjA.OnCollisionEnter(infoB);
		gameObjB.OnCollisionEnter(infoA);
		break;

	case CollisionTypeJudge::ColType::Stay: // �Փ˒��Ȃ�
		gameObjA.OnCollisionStay(infoB);
		gameObjB.OnCollisionStay(infoA);
		break;

	default:
		assert(!"�͈͊O�̒l�����͂���Ă��܂�");
		break;
	}

	return 0;
}
