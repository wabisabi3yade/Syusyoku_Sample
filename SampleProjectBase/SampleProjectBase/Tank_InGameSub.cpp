#include "pch.h"
#include "Tank_InGameSub.h"
#include "PlayerMove.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CameraMove.h"
#include "CameraInput.h"
Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	std::vector<ModelSettings> modelSets =
	{
		ModelSettings("assets/model/tank/uploads_files_4079647_tank+00a.fbx", "Tank01", 0.0025f, true),
		ModelSettings("assets/model/spot/spot.fbx", "Spot01", 1.0f, true),
	};
	// ƒ[ƒhˆ—‚ð‚·‚é
	for (auto m : modelSets)
	{
		ModelLoader::Load(m);
	}

	Model* model = nullptr;
	model = resourceCollection->GetResource<Model>("Md_Tank01");

	std::unique_ptr<Object_3D> object = std::make_unique<Object_3D>();
	object->SetModel(model);
	object->AddComponent<PlayerMove>();
	object->AddComponent<BoxCollider>();
	GameObject* tank = object.get();
	sceneObjects->SetObject("Tank", std::move(object));

	GameObject* camera = sceneObjects->GetSceneObject<GameObject>("MainCamera");
	CameraMove* camMove = camera->AddComponent<CameraMove>();
	camMove->SetPlayer(*tank);
	camera->AddComponent<CameraInput>();
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
