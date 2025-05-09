#pragma once

class CP_RigidBody;

namespace HashiTaku
{
	/// @brief コンポーネントに渡す衝突した情報
	struct CollisionInfo
	{
		/// @brief 
		const CP_RigidBody* pRigidBodyCp{ nullptr };

		/// @brief 衝突座標
		DXSimp::Vector3 contactPoint;
	};
}

