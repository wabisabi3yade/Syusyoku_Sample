#include "pch.h"
#include "VariableFrameRate.h"

void VariableFrameRate::Init(u_int setFrameRate)
{
	FPS = setFrameRate;

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

	// 1フレーム時間に達していないなら
	if (count < microSecondsPerFrame) return false;

	// デルタタイム(ms)にする
	deltaTime_ms = static_cast<double>(count * 0.0001f);

	// 更新する
	previousFrameTime = currentTime;

	return true;
}

void VariableFrameRate::ImGuiSetting()
{
#ifdef EDIT
	if (ImGuiMethod::TreeNode("FPS Counter"))
	{
		float fps = static_cast<float>(1.0f / (deltaTime_ms * 0.001f));

		ImGui::Text("Avarage:%lf ms", avarageProcessTime_ms);
		ImGui::Text("%lf FPS", fps);

		ImGui::TreePop();
	}
#endif
}

void VariableFrameRate::ProcessEnd()
{
#ifdef EDIT
	auto processTime = GetNowTime() - previousFrameTime;
	// 0.1nsからmsに変換
	float processTime_ms = processTime * 0.0001f;

	processTimes.push_back(processTime_ms);

	if (static_cast<u_int>(processTimes.size()) < FPS) return;

	// 1秒ごとの平均処理時間を求める
	float sumProcessTime = 0.0f;
	for (auto& p : processTimes)
		sumProcessTime += p;

	avarageProcessTime_ms = sumProcessTime / FPS;

	processTimes.clear();

#endif // EDIT
}

long long VariableFrameRate::GetNowTime()
{
	QueryPerformanceCounter(&liWork);
	return liWork.QuadPart;
}
