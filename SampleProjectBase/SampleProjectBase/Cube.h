#pragma once
#include "BasicObject_Base.h"

// キューブのメッシュを表示するクラス
class Cube : public BasicObject_Base
{
	DirectX::SimpleMath::Vector3 length;	// 長さ

	void Make() override;	// 頂点データなどの作成
public:
	Cube();
	~Cube() override;

	void Draw(Transform& _transform, DirectX::SimpleMath::Color& _color) override;
};