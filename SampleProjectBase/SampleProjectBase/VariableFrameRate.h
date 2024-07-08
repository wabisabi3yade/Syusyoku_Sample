#pragma once

// 可変フレームレートの秒数を計算するクラス
class VariableFrameRate
{
private:
	// 1フレームごとの秒
	double secondsPerFrame;	

	// Δt(s)
	double deltaTime;	

	// 現在時間
	ULONGLONG currentTime;

	// 前のフレームレートが終了したときの時間
	ULONGLONG previousFrameTime;	

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="setFrameRate">対応させたいフレームレート</param>
	VariableFrameRate(uint64_t setFrameRate);
	~VariableFrameRate();

	// 初期化処理
	void Init();	

	// 対応したfpsにするために次のループまで待つ関数
	void Wait();	 

	// Δtを取得（秒で返す）
	float GetDeltaTime()const { return static_cast<float>(deltaTime); }
};

