#pragma once
#include "BasicObject_Base.h"
class Sphere :
    public BasicObject_Base
{
	void Make() override;	// 頂点データなどの作成

public:
	Sphere();
	~Sphere() override;

	void Draw(const Transform& _transform);
	void DebugDraw(Transform& _transform, DirectX::SimpleMath::Color& _color, bool _isWireFrame = false);
};

