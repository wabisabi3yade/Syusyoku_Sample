#pragma once
#include "Component.h"

class CameraMove : public Component
{

public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	virtual void SetParameter();
};

