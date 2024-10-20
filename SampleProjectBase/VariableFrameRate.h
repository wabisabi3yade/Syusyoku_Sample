#pragma once

// 可変フレームレートの秒数を計算するクラス
class VariableFrameRate : public HashiTaku::IImGuiUser
{
private:
	long long microSecondsPerFrame;	// 1フレームごとのマイクロ秒
	double deltaTime_ms;	// Δt（ms）
	long long previousFrameTime;	// 前のフレームレートが終了したときの時間

	float avarageProcessTime_ms;	// 1秒ごとの平均処理時間
	std::vector<float> processTimes;	// 〇秒ごとの平均をとるための保存リスト

	u_int FPS;	// fps

	// FPS固定用変数
	LARGE_INTEGER liWork; // 関数から値を受け取る用
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="setFrameRate">対応させたいフレームレート</param>
	VariableFrameRate() : microSecondsPerFrame(0), deltaTime_ms(0), previousFrameTime(0), avarageProcessTime_ms(0.0f), FPS(0), liWork() {}

	~VariableFrameRate() {}

	void Init(u_int setFrameRate);	// 初期化処理

	// 対応したfpsにするために次のループまで待つ関数
	bool UpdateCheck();

	/// @brief 処理終了時に呼び出す
	void ProcessEnd();

	// Δtを取得（秒で返す）
	float GetDeltaTime()const { return static_cast<float>(deltaTime_ms * 0.001f); }

private:
	/// @brief 現在時間を取得する
	/// @return 現在時間（0.1ns/単位）
	long long GetNowTime();

	// フレームレート描画
	void ImGuiSetting() override;
};

