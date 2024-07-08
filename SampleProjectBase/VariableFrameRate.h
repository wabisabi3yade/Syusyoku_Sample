#pragma once

// 可変フレームレートの秒数を計算するクラス
class VariableFrameRate
{
private:
	uint64_t microSecondsPerFrame;	// 1フレームごとのマイクロ秒
	uint64_t deltaTime;	// Δt（ミリ秒）
	uint64_t currentTime;	// 現在時間
	uint64_t previousFrameTime;	// 前のフレームレートが終了したときの時間

	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime, endTime;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="setFrameRate">対応させたいフレームレート</param>
	VariableFrameRate(uint64_t setFrameRate);
	~VariableFrameRate();

	void Init();	// 初期化処理

	void CaluculateDelta();	// Δtを計算する

	// 対応したfpsにするために次のループまで待つ関数
	void Wait();	 

	// Δtを取得（秒で返す）
	float GetDeltaTime()const { return deltaTime / 1000.0f; }
};

