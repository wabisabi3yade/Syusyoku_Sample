#pragma once

namespace HashiTaku
{
	namespace Bullet
	{
		/// @brief �R���|�[�l���g�ɓn���Փ˂������
		struct CollisionInfo
		{
			/// @brief 
			const CP_RigidBody* pRigidBodyCp;

			/// @brief �Փˍ��W
			DirectX::SimpleMath::Vector3 contactPoint;
		};
	}
}

