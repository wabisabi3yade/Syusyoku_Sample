#include "pch.h"
#include "Tank_InGameSub.h"
#include "PlayerMove.h"
#include "BoxCollider.h"

Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	std::vector<ModelSettings> modelSets =
	{
		ModelSettings("assets/model/tank/Tank02.fbx", "Tank", false),
	};
	// ƒ[ƒhˆ—‚ð‚·‚é
	for (auto m : modelSets)
	{
		ModelLoader::Load(m);
	}

	Model* model = nullptr;
	model = resourceCollection->GetResource<Model>("Md_Tank");

	std::unique_ptr<Object_3D> object = std::make_unique<Object_3D>();
	object->SetModel(model);
	object->AddComponent<PlayerMove>();
	object->AddComponent<BoxCollider>();

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
