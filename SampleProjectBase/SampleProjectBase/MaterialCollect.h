#pragma once
#include "Singleton_Base.h"
#include "ResourceCollect_Base.h"

// マテリアルを保管するクラス
class MaterialCollect : public Singleton_Base<MaterialCollect>
{
	friend class Singleton_Base<MaterialCollect>;
	//~MaterialCollect() {};
};

