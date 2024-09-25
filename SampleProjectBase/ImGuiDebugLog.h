#pragma once
#include "JISToUTF8.h"	

#ifdef EDIT
#define HASHI_DEBUG_LOG(message) ImGuiDebugLog::Add(message, std::string(__FILE__) , __LINE__)
#else
#define HASHI_DEBUG_LOG(message) 
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
public:
	/// @brief デバッグログに追加する
	/// @param _debugMessage デバッグメッセージ
	/// @param _pathName 書き込んだファイル名
	/// @param _lineNum 書き込んだ行数
	static void Add(const std::string& _debugMessage, const std::string& _pathName, u_int _lineNum);

	/// <summary>
	/// デバッグログを表示させる
	/// </summary>
	static void DisplayMessage();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Terminal();

private:
	ImGuiDebugLog() {};
	~ImGuiDebugLog() { Terminal(); };

	static void ClearMessage();
};

