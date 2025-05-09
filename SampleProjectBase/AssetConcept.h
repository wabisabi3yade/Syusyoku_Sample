#pragma once

// アセット
#include "Asset_Base.h"

// 橋本拓実の名前空間
namespace HashiTaku
{
	// コンセプトで取得できるリソースの型を指定
	template<typename T>
	concept AssetConcept = std::is_base_of_v<Asset_Base, T>;
}