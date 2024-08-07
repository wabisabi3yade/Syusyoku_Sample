#include "pch.h"
#include "Tank_InGameSub.h"

constexpr u_int SPHERE_NUM(1);
constexpr u_int CUBE_NUM(2);

using namespace DirectX::SimpleMath;
using namespace SceneFunction;

Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	std::vector<Texture*> pTextures;

	std::vector<std::string> texturePath =
	{
		{"assets/texture/Grass.jpg"}
	};
	for (auto texPath : texturePath)
	{
		Texture* pTex = AssetLoader::TextureLoad(texPath);
		pTextures.push_back(pTex);
	}

	struct ModelData
	{
		std::string path;
		float scale;
		bool isLeftHand;
		bool isGetScale;
	};

	std::vector<ModelData> SMPaths =
	{
		{"assets/model/spot/spot.fbx", 100.0f, true},
		{"assets/model/knight/Knight D Pelegrini.fbx", 0.1f, true},
		{"assets/model/knight/The Boss.fbx", 0.1f, true}
	};

	std::vector<Mesh_Group*> pModels;
	for (auto& a : SMPaths)
	{
		Mesh_Group* pSM = AssetLoader::ModelLoad(a.path, a.scale, a.isLeftHand);
		pModels.push_back(pSM);
	};

	struct AnimData
	{
		std::string path;
		std::string boneName;
		bool isLeftHand;
	};

	std::vector<AnimData> AnimPath =
	{
		{"assets/animation/Run_Forward.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Walk_Forward.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Idle.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Reaction.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Silly Dancing.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Smash.fbx", "Knight D Pelegrini", true},
	};

	for (auto& a : AnimPath)
	{
		AssetLoader::AnimationLoad(a.path, a.boneName, a.isLeftHand);
	};

	

	GameObject* gameObject = &ObjectFunc::CreateEmpty("Ground");
	CP_SpriteRenderer* pSpriteRenderer = gameObject->AddComponent<CP_SpriteRenderer>();
	pSpriteRenderer->SetTexture(*pTextures[0]);
	gameObject->transform.scale = Vector3::One * 50.0f;

	gameObject = &ObjectFunc::CreateEmpty("Player");
	gameObject->AddComponent<CP_Player>();

	gameObject = &ObjectFunc::CreateEmpty("Box1");
	gameObject->AddComponent<CP_EaseTest>();

	//gameObject = &ObjectFunc::CreateEmpty("Box2");
	//gameObject->AddComponent<CP_BoxCollider>();

	// ŒõŒ¹
	DirectionLight* dirL = LightFunc::CreateDirection();
	dirL->SetDirection(Vector3(1, -1, -1));

	
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
