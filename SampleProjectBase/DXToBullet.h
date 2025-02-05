#pragma once

#include <btBulletDynamicsCommon.h>

// Bulletの変数とDirectXの変換関数
// Bulletは右手系なので考慮する
namespace HashiTaku
{
	namespace Bullet
	{
		namespace DX = DXSimp;

		btScalar ToBtScalar(float _dxScalar);

		/// SimpleMath → Bullet
		btVector3 ToBtVector3(const DX::Vector3& _dxVector3);
		btQuaternion ToBtQuaeternion(const DX::Quaternion& _dxQuaternion);

		/// Bullet → SimpleMath
		float ToDxScalar(btScalar _btScalar);
		DX::Vector3 ToDXVector3(const btVector3& _btVector3);
		DX::Quaternion ToDXQuaternion(const btQuaternion& _btQuaternion);
	}
}