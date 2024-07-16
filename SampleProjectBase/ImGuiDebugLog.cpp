#include "pch.h"
#include "ImGuiDebugLog.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

std::list<ImGuiDebugLog::Message> ImGuiDebugLog::displayList = {};

constexpr u_int MAX_DISPLAY = (1000); // �f�o�b�O���O�\���ő吔

namespace fs = std::filesystem;
void ImGuiDebugLog::Add(const std::string& _debugMessage)
{
#ifdef EDIT
	std::string s =  ShiftJisToUtf8(_debugMessage);	// UTF-8�ɕϊ�

	// ���X�g�̒��Ɋ��ɓ������b�Z�[�W������Ȃ�
	// ��������Ɏ����Ă���悤�ɂ���
	auto itr = std::find_if(displayList.begin(), displayList.end(), [&](Message check)
		{
			return check.dubugMessage == s;	// ���ɂ���̂�
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
		m.dubugMessage = s;
		displayList.push_back(m);
	}
#endif
}

void ImGuiDebugLog::Add(const std::string& _debugMessage, const std::string& _pathName, u_int _lineNum)
{
#ifdef EDIT

	// UTF-8�ɕϊ�
	std::string u8Message = ShiftJisToUtf8(_debugMessage);	

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
	}

#endif

}

void ImGuiDebugLog::DisplayMessage()
{
#ifdef EDIT
	using Font = ImGuiMethod::FontSize;

	ImGuiMethod::SetFontSize(Font::Small);

	ImGui::Begin(ShiftJisToUtf8("�f�o�b�O���O").c_str());

	// ���X�g���̃��b�Z�[�W��\��������
	for (auto d : displayList)
	{	
		std::string debugText;

		debugText = d.fileName +  " " + std::to_string(d.lineNum) + " ";

		// ���e�@+ ����Ăяo���ꂽ�̂�
		debugText += d.dubugMessage + "\t[" + std::to_string(d.writeNum) + "]";
		ImGui::Text(debugText.c_str());
	}

	// �X�N���[�����ŐV�̃��b�Z�[�W�ɍ��킹��
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGuiMethod::PopFontSize();

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
