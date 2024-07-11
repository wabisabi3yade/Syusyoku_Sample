#pragma once
#include "Mesh.h"

// ワイヤーフレームのキューブ
class DebugCube : public Mesh
{
	/// @brief  キューブを作成
	void Make();	
public:
	DebugCube();
	~DebugCube() {}
};

