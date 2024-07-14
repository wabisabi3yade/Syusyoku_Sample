#pragma once
#include "SingleMesh.h"

// キューブメッシュクラス
class Cube : public SingleMesh
{
	/// @brief キューブを作成
	void Make();
public:

	Cube();
	~Cube() {};
};