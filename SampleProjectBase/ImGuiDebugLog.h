#pragma once
#include "JISToUTF8.h"	

#ifdef EDIT
#define HASHI_DEBUG_LOG(message) ImGuiDebugLog::Add(message, std::string(__FILE__) , __LINE__)
#else
#define HASHI_DEBUG_LOG(message) 
#endif


// ImGui�Ńf�o�b�O���O��\��������N���X(static�N���X)
class ImGuiDebugLog
{
	struct Message
	{
		int writeNum;	// �����o����
		std::string dubugMessage;	// ���e
		std::string fileName;	// �t�@�C����
		u_int lineNum;	// �s��
	};	

	// ���t���[���Ńf�o�b�O���O�ɕ\�����郊�X�g
	static std::list<Message> displayList;
public:
	/// @brief �f�o�b�O���O�ɒǉ�����
	/// @param _debugMessage �f�o�b�O���b�Z�[�W
	/// @param _pathName �������񂾃t�@�C����
	/// @param _lineNum �������񂾍s��
	static void Add(const std::string& _debugMessage, const std::string& _pathName, u_int _lineNum);

	/// <summary>
	/// �f�o�b�O���O��\��������
	/// </summary>
	static void DisplayMessage();

	/// <summary>
	/// �I������
	/// </summary>
	static void Terminal();

private:
	ImGuiDebugLog() {};
	~ImGuiDebugLog() { Terminal(); };

	static void ClearMessage();
};

