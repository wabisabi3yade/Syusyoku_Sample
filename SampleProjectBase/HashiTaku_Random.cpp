#include "pch.h"
#include "HashiTaku_Random.h"

namespace HashiTaku
{
	std::mt19937 Random::gen;

	void Random::Init()
	{
		// �V�[�h�𐶐����A�ݒ�
		std::random_device rd;
		gen.seed(rd());
	}
}