#include "pch.h"
#include "HashiTaku_Random.h"

std::mt19937 HashiTaku::Random::gen;

void HashiTaku::Random::Init()
{
	// �V�[�h�𐶐����A�ݒ�
	std::random_device rd;
	gen.seed(rd());
}
