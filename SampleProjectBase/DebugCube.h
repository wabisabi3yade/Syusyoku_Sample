#pragma once
#include "BasicObject_Base.h"
class DebugCube :
    public BasicObject_Base
{
	void Make() override;	// ���_�f�[�^�Ȃǂ̍쐬
public:
	DebugCube();
	~DebugCube() override;

	void Draw(Transform& _transform, DirectX::SimpleMath::Color& _color);
};

