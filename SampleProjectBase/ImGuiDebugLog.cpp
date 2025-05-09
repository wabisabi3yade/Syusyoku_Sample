#include "pch.h"
#include "ImGuiDebugLog.h"

std::list<ImGuiDebugLog::Message> ImGuiDebugLog::displayList = {};

constexpr u_int MAX_DISPLAY = (100); // デバッグログ表示最大数

namespace fs = std::filesystem;

void ImGuiDebugLog::Add(const std::string& _debugMessage, const std::string& _pathName, u_int _lineNum)
{
#ifdef EDIT

	// UTF-8に変換
	std::string u8Message = TO_UTF8(_debugMessage);

	// エラーが出たファイル名を取得
	fs::path filePath = _pathName;
	std::string fileName = filePath.filename().string();

	// リストの中に既に同じメッセージがあるなら
	// それを下に持ってくるようにする
	auto itr = std::find_if(displayList.begin(), displayList.end(), [&](Message check)
		{
			return (check.dubugMessage == u8Message && check.fileName == fileName &&
				check.lineNum == _lineNum);	// 既にあるのか
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
		m.dubugMessage = u8Message;
		m.fileName = fileName;
		m.lineNum = _lineNum;
		displayList.push_back(m);

		if (static_cast<u_int>(displayList.size()) > MAX_DISPLAY)
			displayList.erase(displayList.begin());
	}

#endif

}

void ImGuiDebugLog::DisplayMessage()
{
#ifdef EDIT

	ImGui::Begin(TO_UTF8("デバッグログ"));

	// リスト内のメッセージを表示させる
	for (auto d : displayList)
	{
		std::string debugText;

		debugText = d.fileName + " " + std::to_string(d.lineNum) + " ";

		// 内容　+ 何回呼び出されたのか
		debugText += d.dubugMessage + "\t[" + std::to_string(d.writeNum) + "]";
		ImGui::Text(debugText.c_str());
	}

	if (ImGui::Button("clear"))
		ClearMessage();

	// スクロールを最新のメッセージに合わせる
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::End();
#endif
}

void ImGuiDebugLog::Terminal()
{
#ifdef _DEBUG
	// 終了処理
	displayList.clear();

#endif
}

void ImGuiDebugLog::ClearMessage()
{
	displayList.clear();
}
