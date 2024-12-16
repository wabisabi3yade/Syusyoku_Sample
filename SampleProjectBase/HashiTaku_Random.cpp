#include "pch.h"
#include "HashiTaku_Random.h"

namespace HashiTaku
{
	std::mt19937 Random::gen;

	void Random::Init()
	{
		// シードを生成し、設定
		std::random_device rd;
		gen.seed(rd());
	}
}