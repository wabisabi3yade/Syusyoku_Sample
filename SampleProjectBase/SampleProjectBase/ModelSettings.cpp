#include "ModelSettings.h"

ModelSettings::ModelSettings() : modelName("")
{
}

ModelSettings::~ModelSettings()
{
}

void ModelSettings::Setup(const char* _modelPath, std::string _modelName,
	bool _isRighthand, bool _isPermanent)
{
	modelPath = _modelPath;
	modelName = _modelName;
	isRighthand = _isRighthand;
	isPermanent = _isPermanent;
}
