#include "pch.h"
#include "DXToBullet.h"

namespace HashiTaku
{
	btScalar Bullet::ToBtScalar(float _dxScalar)
	{
		return static_cast<btScalar>(_dxScalar);
	}

	btVector3 Bullet::ToBtVector3(const DX::Vector3& _dxVector3)
	{
		return btVector3(
			ToBtScalar(_dxVector3.x),
			ToBtScalar(_dxVector3.y),
			ToBtScalar(-_dxVector3.z)
		);
	}

	btQuaternion Bullet::ToBtQuaeternion(const DX::Quaternion& _dxQuaternion)
	{
		return btQuaternion(
			ToBtScalar(-_dxQuaternion.x),
			ToBtScalar(-_dxQuaternion.y),
			ToBtScalar(_dxQuaternion.z),
			ToBtScalar(_dxQuaternion.w)
		);
	}

	float Bullet::ToDxScalar(btScalar _btScalar)
	{
		return static_cast<float>(_btScalar);
	}

	DXSimp::Vector3 Bullet::ToDXVector3(const btVector3& _btVector3)
	{
		return DX::Vector3(
			ToDxScalar(_btVector3.getX()),
			ToDxScalar(_btVector3.getY()),
			ToDxScalar(-_btVector3.getZ())
		);
	}

	DXSimp::Quaternion Bullet::ToDXQuaternion(const btQuaternion& _btQuaternion)
	{
		return DX::Quaternion(
			ToDxScalar(-_btQuaternion.getX()),
			ToDxScalar(-_btQuaternion.getY()),
			ToDxScalar(_btQuaternion.getZ()),
			ToDxScalar(_btQuaternion.getW())
		);
	}
}