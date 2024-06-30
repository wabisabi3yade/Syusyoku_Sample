#pragma once

#include "JISToUTF8.h"	
// ImGui�Ńf�o�b�O���O��\��������N���X(static�N���X)
class ImGuiDebugLog
{
	struct Message
	{
		int writeNum;	// �����o����
		std::string dubugMessage;	// ���e
	};	

	// ���t���[���Ńf�o�b�O���O�ɕ\�����郊�X�g
	static std::list<Message> displayList;

	ImGuiDebugLog() {};
	~ImGuiDebugLog() { Terminate(); };
public:

	/// <summary>
	/// �f�o�b�O���O�ɒǉ�����
	/// </summary>
	/// <param name="_debugMessage">�f�o�b�O�ŕ\�����郁�b�Z�[�W</param>
	static void Add(const std::string& _debugMessage);

	/// <summary>
	/// �f�o�b�O���O��\��������
	/// </summary>
	static void DisplayMessage();

	/// <summary>
	/// �I������
	/// </summary>
	static void Terminate();
};

