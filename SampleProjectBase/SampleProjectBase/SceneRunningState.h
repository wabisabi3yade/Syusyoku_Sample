#pragma once

// シーンの稼働状態クラス
class SceneRunningState
{
public:
	// このシーンの稼働状態
	enum RUNNING_STATE
	{
		RUNNING,	// 稼働中
		FINISH,	// 終了（違うシーンに移動する）
		RUNNING_NUM
	};

private:
	RUNNING_STATE runState;	// 現在の稼働状態変数

public:
	SceneRunningState();
	~SceneRunningState();

	// 初期化する
	void Init();

	// シーン遷移するときに稼働状態を終了に変換する関数
	void SetFinshState();

	// 現在の稼働状態を取得する関数
	RUNNING_STATE GetRunningState()const { return runState; }

};

