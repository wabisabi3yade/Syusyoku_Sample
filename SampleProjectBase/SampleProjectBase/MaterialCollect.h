#pragma once
#include "Singleton_Base.h"
#include "ResourceCollect_Base.h"

class Material;

// マテリアルを保管するクラス
class MaterialCollect : public Singleton_Base<MaterialCollect>, public ResourceCollect_Base<Material>
{
	friend class Singleton_Base<MaterialCollect>;
	//~MaterialCollect() {};
};

