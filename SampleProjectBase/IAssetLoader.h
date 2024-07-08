#pragma once

// アセットをロードするインターフェース
class IAssetLoader 
{

public:
	virtual bool Load(const char* _filePath) = 0;
};

