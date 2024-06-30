#include "pch.h"
#include "VariableFrameRate.h"

#include <thread>
#include <chrono>

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

void VariableFrameRate::CaluculateDelta()
{
	currentTime = GetTickCount64();				// 現在時刻取得
	deltaTime = currentTime - previousFrameTime;	// 前回実行時からに経過時間を取得
	previousFrameTime = currentTime;
}

VariableFrameRate::VariableFrameRate(uint64_t setFrameRate) : deltaTime(0), currentTime(0),
previousFrameTime(0)
{
	microSecondsPerFrame = (1000 * 1000) / setFrameRate;	// 1フレームのマイクロ秒数を求める

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
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
	// 次のフレームまでに待機する時間を求める
	int64_t waitTime = microSecondsPerFrame - deltaTime;

	if (waitTime > 0)
	{
		float tt = waitTime / 1000.0f;

		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));	// スレッドを待機させる
	}
}
