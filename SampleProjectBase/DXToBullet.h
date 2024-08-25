#pragma once

#include <btBulletDynamicsCommon.h>

// Bullet‚Ì•Ï”‚ÆDirectX‚Ì•ÏŠ·ŠÖ”
// Bullet‚Í‰EèŒn‚È‚Ì‚Ål—¶‚·‚é
namespace HashiTaku
{
	namespace Bullet
	{
		namespace DX = DirectX::SimpleMath;

		btScalar ToBtScalar(float _dxScalar);

		/// SimpleMath ¨ Bullet
		btVector3 ToBtVector3(const DX::Vector3& _dxVector3);
		btQuaternion ToBtQuaeternion(const DX::Quaternion& _dxQuaternion);

		/// Bullet ¨ SimpleMath
		float ToDxScalar(btScalar _btScalar);
		DX::Vector3 ToDXVector3(const btVector3& _btVector3);
		DX::Quaternion ToDXQuaternion(const btQuaternion& _btQuaternion);
	}
}