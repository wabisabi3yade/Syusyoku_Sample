#include "pch.h"
#include "ImGuiDebugLog.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

std::list<ImGuiDebugLog::Message> ImGuiDebugLog::displayList = {};

constexpr u_int MAX_DISPLAY = (1000); // デバッグログ表示最大数

void ImGuiDebugLog::Add(const std::string& _debugMessage)
{
#ifdef EDIT
	std::string s =  ShiftJisToUtf8(_debugMessage);	// UTF-8に変換

	// リストの中に既に同じメッセージがあるなら
	// それを下に持ってくるようにする
	auto itr = std::find_if(displayList.begin(), displayList.end(), [&](Message check)
		{
			return check.dubugMessage == s;	// 既にあるのか
		});

	if (itr != displayList.end())	// 既にあったら
	{
		itr->writeNum++;	// 書き込み回数を1増やす
		// 末尾に移動する
		displayList.splice(displayList.end(), displayList, itr);
		
	}
	else	// ないなら
	{
		// リストに追加する
		Message m;
		m.writeNum = 1;
		m.dubugMessage = s;
		displayList.push_back(m);
	}
#endif
}

void ImGuiDebugLog::DisplayMessage()
{
#ifdef EDIT
	ImGui::Begin(ShiftJisToUtf8("デバッグログ").c_str());
	
	// リスト内のメッセージを表示させる
	for (auto d : displayList)
	{	
		// 内容　+ 何回呼び出されたのか
		std::string debugText = d.dubugMessage + "\t[" + std::to_string(d.writeNum) + "]";
		ImGui::Text(debugText.c_str());
	}

	// スクロールを最新のメッセージに合わせる
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::End();
#endif
}

void ImGuiDebugLog::Terminate()
{
#ifdef _DEBUG
	// 終了処理
	displayList.clear();
	
#endif
}
