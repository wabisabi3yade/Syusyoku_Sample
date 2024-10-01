#include "pch.h"
#include "ImGuiDebugLog.h"

std::list<ImGuiDebugLog::Message> ImGuiDebugLog::displayList = {};

constexpr u_int MAX_DISPLAY = (100); // �f�o�b�O���O�\���ő吔

namespace fs = std::filesystem;

void ImGuiDebugLog::Add(const std::string& _debugMessage, const std::string& _pathName, u_int _lineNum)
{
#ifdef EDIT

	// UTF-8�ɕϊ�
	std::string u8Message = TO_UTF8(_debugMessage);

	// �G���[���o���t�@�C�������擾
	fs::path filePath = _pathName;
	std::string fileName = filePath.filename().string();

	// ���X�g�̒��Ɋ��ɓ������b�Z�[�W������Ȃ�
	// ��������Ɏ����Ă���悤�ɂ���
	auto itr = std::find_if(displayList.begin(), displayList.end(), [&](Message check)
		{
			return (check.dubugMessage == u8Message && check.fileName == fileName &&
				check.lineNum == _lineNum);	// ���ɂ���̂�
		});

	if (itr != displayList.end())	// ���ɂ�������
	{
		itr->writeNum++;	// �������݉񐔂�1���₷
		// �����Ɉړ�����
		displayList.splice(displayList.end(), displayList, itr);
	}
	else	// �Ȃ��Ȃ�
	{
		// ���X�g�ɒǉ�����
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

	ImGui::Begin(TO_UTF8("�f�o�b�O���O"));

	// ���X�g���̃��b�Z�[�W��\��������
	for (auto d : displayList)
	{
		std::string debugText;

		debugText = d.fileName + " " + std::to_string(d.lineNum) + " ";

		// ���e�@+ ����Ăяo���ꂽ�̂�
		debugText += d.dubugMessage + "\t[" + std::to_string(d.writeNum) + "]";
		ImGui::Text(debugText.c_str());
	}

	if (ImGui::Button("clear"))
		ClearMessage();

	// �X�N���[�����ŐV�̃��b�Z�[�W�ɍ��킹��
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::End();
#endif
}

void ImGuiDebugLog::Terminal()
{
#ifdef _DEBUG
	// �I������
	displayList.clear();

#endif
}

void ImGuiDebugLog::ClearMessage()
{
	displayList.clear();
}
