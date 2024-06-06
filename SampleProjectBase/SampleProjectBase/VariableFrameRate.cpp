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
	previousFrameTime = GetTickCount64();
}

VariableFrameRate::VariableFrameRate(uint64_t setFrameRate) : deltaTime(0), currentTime(0),
previousFrameTime(0)
{
	microSecondsPerFrame = (1000 * 1000) / setFrameRate;	// 1フレームのマイクロ秒数を求める
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

	// 次のフレームまでに待機する時間を求める
	float waitTime = static_cast<float>(microSecondsPerFrame) - deltaTime * 1000;

	if (waitTime > 0)
	{
		waitTime /= 1000.0f;

		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(waitTime)));	// スレッドを待機させる
	}
}
