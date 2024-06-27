#include "pch.h"
#include "Tank_InGameSub.h"
#include "PlayerMove.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CameraMove.h"
#include "CameraInput.h"

#include "Field.h"
Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	std::vector<ModelSettings> modelSets =
	{
		ModelSettings("assets/model/tank/Tank_Toon.fbx", "Tank01", 1.0f, true),
		ModelSettings("assets/model/spot/spot.fbx", "Spot01", 1.0f, true),
	};
	// ロード処理をする
	for (auto m : modelSets)
	{
		ModelLoader::Load(m);
	}
	std::vector<TextureLoader::TextureSetting> textureSets =
	{
		{"assets/texture/Grass.png", "Grass01"}
	};
	Model* model = nullptr;
	Texture* tex = nullptr;

	// 戦車
	model = resourceCollection->GetResource<Model>("Md_Tank01");
	std::unique_ptr<Object_3D> object = std::make_unique<Object_3D>();
	object->SetModel(model);
	object->AddComponent<PlayerMove>();
	object->AddComponent<BoxCollider>();
	GameObject* tank = object.get();
	sceneObjects->SetObject("Tank", std::move(object));

	// 牛
	model = resourceCollection->GetResource<Model>("Md_Spot01");
	object = std::make_unique<Object_3D>();
	object->SetModel(model);
	sceneObjects->SetObject("Spot", std::move(object));

	// カメラ
	GameObject* camera = sceneObjects->GetSceneObject<GameObject>("MainCamera");
	CameraMove* camMove = camera->AddComponent<CameraMove>();
	camMove->SetPlayer(*tank);
	camera->AddComponent<CameraInput>();

	// 地面
	std::unique_ptr<Field> field = std::make_unique<Field>();
	tex = TextureLoader::Load(textureSets[0]);
	field->SetTexture(tex);
	sceneObjects->SetObject("Field", std::move(field));
}

Tank_InGameSub::~Tank_InGameSub()
{
}

void Tank_InGameSub::Update()
{
	GameObject* spot = sceneObjects->GetSceneObject<GameObject>("Spot");
	GameObject* tank = sceneObjects->GetSceneObject<GameObject>("Tank");
	spot->transform.LookAt(tank->transform.position);
}

void Tank_InGameSub::LateUpdate()
{
}

void Tank_InGameSub::Draw()
{
}
