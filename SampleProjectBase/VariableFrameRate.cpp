#include "pch.h"
#include "VariableFrameRate.h"

void VariableFrameRate::Init(u_int setFrameRate)
{
	// FPS固定用変数
	long long frequency; // 計測精度
	// 計測精度を取得
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart; // １秒あたりの解像度が入る
	// １フレームあたりのカウント値を計算
	microSecondsPerFrame = frequency / setFrameRate; // 60FPSで計算

	previousFrameTime = GetNowTime();
}

bool VariableFrameRate::UpdateCheck()
{
	// 前のフレームからの秒数を測る
	auto currentTime = GetNowTime();
	auto count = currentTime - previousFrameTime;

	// 1フレーム時間に達したら
	if (count < microSecondsPerFrame)
		return false;

	// デルタタイム(ms)にする
	deltaTime = static_cast<double>(count * 0.0001f);

	// 更新する
	previousFrameTime = currentTime;

	return true;
}

void VariableFrameRate::Draw()
{
	if (ImGui::TreeNode("FPS Counter"))
	{
		float fps = static_cast<float>(1.0f / (deltaTime * 0.001f));

		ImGui::Text("%lf FPS", fps);

		ImGui::TreePop();
	}
}

long long VariableFrameRate::GetNowTime()
{
	QueryPerformanceCounter(&liWork);
	return liWork.QuadPart;
}
