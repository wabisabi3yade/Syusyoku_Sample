#include "pch.h"
#include "ResourceCollection.h"

// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"


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

void ResourceCollection::ImportDisplay()
{
#ifdef EDIT

	ImGui::Begin(ShiftJisToUtf8("���\�[�X").c_str());
	for (auto itr = resources.begin(); itr != resources.end(); itr++)
	{
		ImGui::Text(itr->first.c_str());
	}
	ImGui::End();

#endif // EDIT
}
