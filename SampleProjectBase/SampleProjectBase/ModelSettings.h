#pragma once

// モデルをインポートをするときの設定
class ModelSettings
{
public:
	bool isRighthand = false;	// 右手系座標かどうか
	bool isPermanent = false;	// 移動したときに解放されない（ずっと残る）
	float scaleBase;	// インポートするときのモデルの大きさ

	const char* modelPath{ nullptr };	// モデルファイルのパス
	std::string modelName;	// モデルの名前

	ModelSettings(){};
	ModelSettings(const char* _modelPath, std::string _modelName, float _scaleBase,
		bool _isRighthand = false, bool _isPermanent = false);
	~ModelSettings();
};