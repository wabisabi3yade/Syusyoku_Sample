#pragma once
#include "BasicObject_Base.h"

// キューブのメッシュを表示するクラス
class Cube : public Mesh
{
	void Make();	// 頂点データなどの作成
public:
	Cube();
	~Cube() override;
};