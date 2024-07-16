#pragma once
#include "JISToUTF8.h"	

#ifdef EDIT
#define HASHI_DEBUG_LOG(message) ImGuiDebugLog::Add(message, std::string(__FILE__) , __LINE__)
#else
#define HASHI_DEBUG_LOG(message) ((void)0)
#endif


// ImGuiでデバッグログを表示させるクラス(staticクラス)
class ImGuiDebugLog
{
	struct Message
	{
		int writeNum;	// 書き出し回数
		std::string dubugMessage;	// 内容
		std::string fileName;	// ファイル名
		u_int lineNum;	// 行数
	};	

	// 今フレームでデバッグログに表示するリスト
	static std::list<Message> displayList;

	ImGuiDebugLog() {};
	~ImGuiDebugLog() { Terminal(); };
public:

	/// <summary>
	/// デバッグログに追加する
	/// </summary>
	/// <param name="_debugMessage">デバッグで表示するメッセージ</param>
	static void Add(const std::string& _debugMessage);

	///
	static void Add(const std::string& _debugMessage, const std::string& _pathName, u_int _lineNum);

	/// <summary>
	/// デバッグログを表示させる
	/// </summary>
	static void DisplayMessage();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Terminal();
};

