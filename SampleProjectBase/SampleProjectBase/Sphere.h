#pragma once
#include "Mesh.h"

// 球メッシュクラス
class Sphere :
    public Mesh
{
	void Make();	// 頂点データなどの作成

public:
	Sphere();
	~Sphere() override;
};

