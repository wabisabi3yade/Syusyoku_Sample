#pragma once

// モデルをインポートをするときの設定
class ModelSettings
{
public:
	bool isRighthand = false;	// 右手系座標かどうか
	bool isPermanent = false;	// 移動したときに解放されない（ずっと残る）

	const char* modelPath{ nullptr };	// モデルファイルのパス
	std::string modelName;	// モデルの名前

	ModelSettings();
	ModelSettings(const char* _modelPath, std::string _modelName,
		bool _isRighthand = false, bool _isPermanent = false);
	~ModelSettings();

	// モデルをロードする前の準備
	void Setup(const char* _modelPath, std::string _modelName, 
		bool _isRighthand = false, bool _isPermanent = false);
};