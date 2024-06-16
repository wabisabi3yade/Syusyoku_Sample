#include "pch.h"
#include "Tank_InGameSub.h"

Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	Model* model = nullptr;
	ModelSettings modelSetting;
	modelSetting.Setup("assets/model/T 90.fbx", "Tank", false);
	ModelLoader::Load(modelSetting);
	
	std::unique_ptr<Object_3D> object = std::make_unique<Object_3D>();
	model = resourceCollection->GetResource<Model>("Md_Tank");
	object->SetModel(model);
	sceneObjects->SetObject("Tank", std::move(object));
}

Tank_InGameSub::~Tank_InGameSub()
{
}

void Tank_InGameSub::Update()
{
}

void Tank_InGameSub::LateUpdate()
{
}

void Tank_InGameSub::Draw()
{
}
