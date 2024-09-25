#include "pch.h"
#include "HashiTaku_Random.h"

std::mt19937 HashiTaku::Random::gen;

void HashiTaku::Random::Init()
{
	// シードを生成し、設定
	std::random_device rd;
	gen.seed(rd());
}
