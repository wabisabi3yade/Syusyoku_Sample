#pragma once
#include "BasicObject_Base.h"
class DebugCube :
    public BasicObject_Base
{
	void Make() override;	// 頂点データなどの作成
public:
	DebugCube();
	~DebugCube() override;

	void Draw(Transform& _transform, DirectX::SimpleMath::Color& _color);
};

