#pragma once
#include "Mesh.h"

// キューブメッシュクラス
class Cube : public Mesh
{
	/// @brief キューブを作成
	void Make();
public:
	Cube();
	~Cube() {};
};