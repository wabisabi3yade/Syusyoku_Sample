#pragma once
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDebugDraw.h"

namespace HashiTaku
{
	class BulletTest
	{
		btDefaultCollisionConfiguration* collisionConfiguration;

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* dispatcher;

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* overlappingPairCache;

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;

		btAlignedObjectArray<btCollisionShape*> collisionShapes;

		BulletDebugDraw* debugDraw;

	public:
		BulletTest();
		~BulletTest();

		void Update();


	};
}