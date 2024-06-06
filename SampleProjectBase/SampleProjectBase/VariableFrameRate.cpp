#include "pch.h"
#include "VariableFrameRate.h"

#include <thread>
#include <chrono>

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

void VariableFrameRate::CaluculateDelta()
{
	currentTime = GetTickCount64();				// ���ݎ����擾
	deltaTime = currentTime - previousFrameTime;	// �O����s������Ɍo�ߎ��Ԃ��擾
	previousFrameTime = GetTickCount64();
}

VariableFrameRate::VariableFrameRate(uint64_t setFrameRate) : deltaTime(0), currentTime(0),
previousFrameTime(0)
{
	microSecondsPerFrame = (1000 * 1000) / setFrameRate;	// 1�t���[���̃}�C�N���b�������߂�
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
	CaluculateDelta();

	// ���̃t���[���܂łɑҋ@���鎞�Ԃ����߂�
	float waitTime = static_cast<float>(microSecondsPerFrame) - deltaTime * 1000;

	if (waitTime > 0)
	{
		waitTime /= 1000.0f;

		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(waitTime)));	// �X���b�h��ҋ@������
	}
}
