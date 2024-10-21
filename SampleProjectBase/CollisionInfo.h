#pragma once

namespace HashiTaku
{
	namespace Bullet
	{
		/// @brief コンポーネントに渡す衝突した情報
		struct CollisionInfo
		{
			/// @brief 
			const CP_RigidBody* pRigidBodyCp;

			/// @brief 衝突座標
			DirectX::SimpleMath::Vector3 contactPoint;
		};
	}
}

