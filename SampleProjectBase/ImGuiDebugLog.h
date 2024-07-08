#pragma once

#include "JISToUTF8.h"	
// ImGuiでデバッグログを表示させるクラス(staticクラス)
class ImGuiDebugLog
{
	struct Message
	{
		int writeNum;	// 書き出し回数
		std::string dubugMessage;	// 内容
	};	

	// 今フレームでデバッグログに表示するリスト
	static std::list<Message> displayList;

	ImGuiDebugLog() {};
	~ImGuiDebugLog() { Terminate(); };
public:

	/// <summary>
	/// デバッグログに追加する
	/// </summary>
	/// <param name="_debugMessage">デバッグで表示するメッセージ</param>
	static void Add(const std::string& _debugMessage);

	/// <summary>
	/// デバッグログを表示させる
	/// </summary>
	static void DisplayMessage();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Terminate();
};

