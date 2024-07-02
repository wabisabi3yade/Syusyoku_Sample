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
		ModelSettings("assets/model/spot/spot.fbx", "Spot01", 1.0f, true),
	};
	// ���[�h����������
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

	// �J����
	GameObject* camera = sceneObjects->GetSceneObject<GameObject>("MainCamera");
	CameraMove* camMove = camera->AddComponent<CameraMove>();
	camera->AddComponent<CameraInput>();

	// �n��
	std::unique_ptr<Field> field = std::make_unique<Field>();
	tex = TextureLoader::Load(textureSets[0]);
	field->SetTexture(tex);
	sceneObjects->SetObject("Field", std::move(field));

	//�@�f�B���N�V�������C�g
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
