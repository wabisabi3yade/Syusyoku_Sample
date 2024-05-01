#pragma once

// モデルをインポートをするときの設定
class ModelSettings
{
public:
	bool isRighthand = false;	// 右手系座標かどうか

	const char* modelPath{ nullptr };	// モデルファイルのパス

	ModelSettings();
	~ModelSettings();
};