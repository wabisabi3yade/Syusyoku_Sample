#pragma once
#include "SceneLights.h"

// シーン内部のシステムのインスタンスを持つクラス
class InSceneManager : Singleton_Base<InSceneManager>
{
	friend class Singleton_Base<InSceneManager>;

	InSceneManager() : pSceneLights(nullptr){};
	~InSceneManager(){};

	SceneLights* pSceneLights;
public:


};

