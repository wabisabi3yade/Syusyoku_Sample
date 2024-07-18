#pragma once



// 可変フレームレートの秒数を計算するクラス
class VariableFrameRate
{
private:
	long long microSecondsPerFrame;	// 1フレームごとのマイクロ秒
	double deltaTime;	// Δt（ms）
	long long previousFrameTime;	// 前のフレームレートが終了したときの時間

	// FPS固定用変数
	LARGE_INTEGER liWork; // 関数から値を受け取る用
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="setFrameRate">対応させたいフレームレート</param>
	VariableFrameRate() : microSecondsPerFrame(0), deltaTime(0), previousFrameTime(0), liWork() {}
	~VariableFrameRate() {}

	void Init(u_int setFrameRate);	// 初期化処理

	// 対応したfpsにするために次のループまで待つ関数
	bool UpdateCheck();

	// フレームレート描画
	void Draw();

	// Δtを取得（秒で返す）
	float GetDeltaTime()const { return deltaTime * 0.001f ; }

private:

	/// @brief 現在時間を取得する
	/// @return 現在時間（0.1ns/単位）
	long long GetNowTime();
};

