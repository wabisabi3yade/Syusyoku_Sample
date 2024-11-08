#pragma once
#include "Transform.h"

/// @brief nullptr代わりのトランスフォームクラス
class NullTransform : public Transform
{

public:
	NullTransform() : Transform(nullptr, false) { isHaveParent = true; isNull = true; }
	~NullTransform() {}

	void SetChild(Transform& _child) override {}
	void RemoveParent() override {}
	void RemoveChild(Transform& _removeTransform) override {}

	void SetPosition(const DirectX::SimpleMath::Vector3& _pos) override {}
	void SetScale(const DirectX::SimpleMath::Vector3& _scale) override {}
	void SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles) override {}
	void SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion) override {}

	void SetLocalPosition(const DirectX::SimpleMath::Vector3& _position) override {}
	void SetLocalScale(const DirectX::SimpleMath::Vector3& _scale) override {}
	void SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles) override {}
	void SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion) override {}
};

