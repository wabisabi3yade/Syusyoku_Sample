#include "pch.h"
#include "ResourceCollection.h"

bool ResourceCollection::GetImpotred(std::string _resourceName)
{
	// ���O����T��
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())	// ���O���z����ɂ����
	{
		return true;	// �����Ă��邱�Ƃ�Ԃ�
	}

	return false;	// �����Ă��Ȃ����Ƃ�Ԃ�
}
