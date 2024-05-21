#pragma once
#include "ResourceCollect_Base.h"
#include "Singleton_Base.h"

class Model;
class ModelSettings;

class ModelCollect :
	public  ResourceCollect_Base<Model>, public Singleton_Base<ModelCollect>
{
	friend class Singleton_Base<ModelCollect>;

public:
	bool Load(const ModelSettings& _settings);
};

