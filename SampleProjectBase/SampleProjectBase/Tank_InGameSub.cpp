#include "pch.h"
#include "Tank_InGameSub.h"
#include "PlayerMove.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CameraMove.h"
#include "CameraInput.h"
#include "Tank.h"
#include "Field.h"
Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	std::vector<ModelSettings> modelSets =
	{
		ModelSettings("assets/model/tank/Tank_Toon.fbx", "Tank01", 1.0f, true),
		ModelSettings("assets/model/spot/spot.fbx", "Spot01", 1.0f, true),
		ModelSettings("assets/model/tank/Bullet.STL", "Bullet", 1.0f, true),
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
	std::unique_ptr <Tank> tank = std::make_unique<Tank>();
	tank->SetModel(model);
	PixelShader& pixelSh = *ShaderCollection::GetInstance()->GetPixelShader("PS_TexNotEnable");
	tank->GetModel().SetPixelShader(&pixelSh);
	GameObject* tankPtr = tank.get();
	sceneObjects->SetObject("Tank", std::move(tank));

	// 牛
	model = resourceCollection->GetResource<Model>("Md_Bullet01");
	std::unique_ptr<Object_3D> object = std::make_unique<Object_3D>();
	object->SetModel(model);
	sceneObjects->SetObject("Spot", std::move(object));

	// カメラ
	GameObject* camera = sceneObjects->GetSceneObject<GameObject>("MainCamera");
	CameraMove* camMove = camera->AddComponent<CameraMove>();
	camMove->SetPlayer(*tankPtr);
	camera->AddComponent<CameraInput>();

	// 地面
	std::unique_ptr<Field> field = std::make_unique<Field>();
	tex = TextureLoader::Load(textureSets[0]);
	field->SetTexture(tex);
	sceneObjects->SetObject("Field", std::move(field));

	//　ディレクションライト
	std::unique_ptr<DirectionLight> pDirLight = std::make_unique<DirectionLight>();
	sceneLights->SetDirectionLight(std::move(pDirLight));
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
