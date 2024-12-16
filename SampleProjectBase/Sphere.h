#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	// 球メッシュクラス
	class Sphere :
		public SingleMesh
	{
		void Make();	// 頂点データなどの作成

	public:
		Sphere();
		~Sphere() {}
	};
}