#include "pch.h"
#include "ImGuiDebugLog.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <string>

std::list<ImGuiDebugLog::Message> ImGuiDebugLog::displayList = {};

constexpr u_int MAX_DISPLAY = (1000); // �f�o�b�O���O�\���ő吔

void ImGuiDebugLog::AddDebugLog(const std::string& _debugMessage)
{
#ifdef _DEBUG
	// ���X�g�̒��Ɋ��ɓ������b�Z�[�W������Ȃ�
	// ��������Ɏ����Ă���悤�ɂ���
	auto itr = std::find_if(displayList.begin(), displayList.end(), [&](Message check)
		{
			return check.dubugMessage == _debugMessage;	// ���ɂ���̂�
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
		m.dubugMessage = _debugMessage;
		displayList.push_back(m);
	}
#endif
}

void ImGuiDebugLog::DisplayMessage()
{
#ifdef _DEBUG
	ImGui::Begin("DebugLog");
	
	// ���X�g���̃��b�Z�[�W��\��������
	for (auto d : displayList)
	{	
		// ���e�@+ ����Ăяo���ꂽ�̂�
		std::string debugText = d.dubugMessage + "\t[" + std::to_string(d.writeNum) + "]";
		ImGui::Text(debugText.c_str());
	}

	// �X�N���[�����ŐV�̃��b�Z�[�W�ɍ��킹��
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::End();
#endif
}

void ImGuiDebugLog::Terminate()
{
#ifdef _DEBUG
	// �I������
	displayList.clear();
#endif
}
