#pragma once
#include "SceneLights.h"

// �V�[�������̃V�X�e���̃C���X�^���X�����N���X
class InSceneManager : Singleton_Base<InSceneManager>
{
	friend class Singleton_Base<InSceneManager>;

	InSceneManager() : pSceneLights(nullptr){};
	~InSceneManager(){};

	SceneLights* pSceneLights;
public:


};

