#include "pch.h"
#include "VariableFrameRate.h"

#include <thread>
#include <chrono>

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

VariableFrameRate::VariableFrameRate(uint64_t setFrameRate) : deltaTime(0), currentTime(0),
previousFrameTime(0)
{
	secondsPerFrame = 1000.0f / setFrameRate;	// 1�t���[���̃}�C�N���b�������߂�
}

VariableFrameRate::~VariableFrameRate()
{
}

void VariableFrameRate::Init()
{
	previousFrameTime = GetTickCount64();
}

void VariableFrameRate::Wait()
{
	// �O����s������Ɍo�ߎ��Ԃ��擾
	currentTime = GetTickCount64();				
	deltaTime = static_cast<double>(currentTime - previousFrameTime) / 1000.0f;

	// ���̃t���[���܂łɑҋ@���鎞�Ԃ����߂�
	double waitTime = secondsPerFrame - deltaTime;

	if (waitTime > 0)
	{
		// �ҋ@
		Sleep(static_cast<DWORD>(waitTime));
	}

	previousFrameTime = GetTickCount64();
}
