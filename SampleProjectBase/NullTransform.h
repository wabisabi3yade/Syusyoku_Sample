#pragma once
#include "Transform.h"

namespace HashiTaku
{
	/// @brief nullptr代わりのトランスフォームクラス
	class NullTransform : public Transform
	{

	public:
		NullTransform() : Transform(nullptr, false) { isHaveParent = true; isNull = true; }
		~NullTransform() {}

		void SetChild(Transform& _child) override {}
		void RemoveParent() override {}
		void RemoveChild(Transform& _removeTransform) override {}

		void SetPosition(const DXSimp::Vector3& _pos) override {}
		void SetScale(const DXSimp::Vector3& _scale) override {}
		void SetEularAngles(const DXSimp::Vector3& _eularAngles) override {}
		void SetRotation(const DXSimp::Quaternion& _quaternion) override {}

		void SetLocalPosition(const DXSimp::Vector3& _position) override {}
		void SetLocalScale(const DXSimp::Vector3& _scale) override {}
		void SetLocalEularAngles(const DXSimp::Vector3& _eularAngles) override {}
		void SetLocalRotation(const DXSimp::Quaternion& _quaternion) override {}
	};
}