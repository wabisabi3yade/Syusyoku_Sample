#pragma once

class CP_RigidBody;

namespace HashiTaku
{
	/// @brief �R���|�[�l���g�ɓn���Փ˂������
	struct CollisionInfo
	{
		/// @brief 
		const CP_RigidBody* pRigidBodyCp{ nullptr };

		/// @brief �Փˍ��W
		DXSimp::Vector3 contactPoint;
	};
}

