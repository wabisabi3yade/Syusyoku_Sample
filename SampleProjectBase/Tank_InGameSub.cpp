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

	std::vector<Mesh_Group*> pModels;

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
	};

	struct AnimData
	{
		std::string path;
		bool isLeftHand;
	};

	std::vector<AnimData> AnimPath =
	{
		{"assets/animation/Silly Dancing.fbx", true},
		{"assets/animation/Akai_Run.fbx", true},
	};

	for (auto& a : AnimPath)
	{
		AssetLoader::AnimationLoad(a.path, a.isLeftHand);
	};

	Mesh_Group* pSM = AssetLoader::ModelLoad(SMPaths[1].path, SMPaths[1].scale, SMPaths[1].isLeftHand);
	pModels.push_back(pSM);

	GameObject* gameObject = &ObjectFunc::CreateEmpty("Ground");
	CP_SpriteRenderer* pSpriteRenderer = gameObject->AddComponent<CP_SpriteRenderer>();
	pSpriteRenderer->SetTexture(*pTextures[0]);
	gameObject->transform.scale = Vector3::One * 50.0f;

	gameObject = &ObjectFunc::CreateEmpty("knight");
	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh(*pModels[0]);
	pMeshRenderer->SetVertexShader("VS_SkinAnimation");
	pMeshRenderer->SetPixelShader("PS_Unlit");
	CP_Animation* pAnimation = gameObject->AddComponent<CP_Animation>();
	pAnimation->AddAnimations("Silly Dancing");
	pAnimation->AddAnimations("Akai_Run");

	/*gameObject = &ObjectFunc::CreateEmpty("Box1");
	gameObject->AddComponent<CP_BoxCollider>();

	gameObject = &ObjectFunc::CreateEmpty("Box2");
	gameObject->AddComponent<CP_BoxCollider>();*/

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
