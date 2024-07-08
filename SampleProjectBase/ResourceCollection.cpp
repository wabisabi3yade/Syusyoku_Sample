#include "pch.h"
#include "ResourceCollection.h"

// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"


bool ResourceCollection::GetImpotred(std::string _resourceName)
{
	// 名前から探す
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())	// 名前が配列内にあれば
	{
		return true;	// 入っていることを返す
	}

	return false;	// 入っていないことを返す
}

void ResourceCollection::ImportDisplay()
{
#ifdef EDIT

	ImGui::Begin(ShiftJisToUtf8("リソース").c_str());
	for (auto itr = resources.begin(); itr != resources.end(); itr++)
	{
		ImGui::Text(itr->first.c_str());
	}
	ImGui::End();

#endif // EDIT
}
