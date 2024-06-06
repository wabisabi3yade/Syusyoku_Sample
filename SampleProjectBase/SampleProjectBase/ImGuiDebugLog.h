#pragma once

// ImGuiでデバッグログを表示させるクラス
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
	~ImGuiDebugLog(){};
public:

	/// <summary>
	/// デバッグログに追加する
	/// </summary>
	/// <param name="_debugMessage">デバッグで表示するメッセージ</param>
	static void AddDebugLog(const std::string& _debugMessage);

	/// <summary>
	/// デバッグログを表示させる
	/// </summary>
	static void DisplayMessage();

	static void Terminate();
};

