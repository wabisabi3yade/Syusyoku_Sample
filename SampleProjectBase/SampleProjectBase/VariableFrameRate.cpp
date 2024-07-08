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
	secondsPerFrame = 1000.0f / setFrameRate;	// 1フレームのマイクロ秒数を求める
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
	// 前回実行時からに経過時間を取得
	currentTime = GetTickCount64();				
	deltaTime = static_cast<double>(currentTime - previousFrameTime) / 1000.0f;

	// 次のフレームまでに待機する時間を求める
	double waitTime = secondsPerFrame - deltaTime;

	if (waitTime > 0)
	{
		// 待機
		Sleep(static_cast<DWORD>(waitTime));
	}

	previousFrameTime = GetTickCount64();
}
