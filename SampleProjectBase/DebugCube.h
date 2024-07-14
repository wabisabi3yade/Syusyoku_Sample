#pragma once
#include "SingleMesh.h"

// ワイヤーフレームのキューブ
class DebugCube : public SingleMesh
{
	/// @brief  キューブを作成
	void Make();	
public:
	DebugCube();
	~DebugCube() {}
};

