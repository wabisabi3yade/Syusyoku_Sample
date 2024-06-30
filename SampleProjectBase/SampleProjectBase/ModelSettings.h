#pragma once

// モデルをインポートをするときの設定
class ModelSettings
{
public:
	bool isRighthand;	// 右手系座標かどうか
	bool isPermanent;	// 移動したときに解放されない（ずっと残る）
	float scaleBase;	// インポートするときのモデルの大きさ

	const char* modelPath;	// モデルファイルのパス
	std::string modelName;	// モデルの名前

	ModelSettings(): isRighthand(false), isPermanent(false), scaleBase(1.0f), modelPath(nullptr), modelName("") {};
	ModelSettings(const char* _modelPath, std::string _modelName, float _scaleBase,
		bool _isRighthand = false, bool _isPermanent = false);
	~ModelSettings();
};