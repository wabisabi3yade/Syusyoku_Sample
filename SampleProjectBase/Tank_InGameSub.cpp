#include "pch.h"
#include "Tank_InGameSub.h"

constexpr u_int SPHERE_NUM(1);
constexpr u_int CUBE_NUM(2);

using namespace DXSimp;
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
		Vector3 rotation;
		bool isLeftHand;
		bool isGetScale;
	};

	std::vector<ModelData> SMPaths =
	{
		{"assets/model/spot/spot.fbx", 1.0f, Vector3(0,0.f, 0), true},
		{"assets/model/knight/Knight D Pelegrini.fbx", 1.0f, Vector3(0,0.0f, 0),true},
	};

	std::vector<Mesh_Group*> pModels;
	for (auto& a : SMPaths)
	{
		Mesh_Group* pSM = AssetLoader::ModelLoad(a.path, a.scale, a.rotation, a.isLeftHand);
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
		{"assets/animation/Player/RunF.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Player/WalkF.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Player/Idle.fbx", "Knight D Pelegrini", true},
		{"assets/animation/Player/AttackN1.fbx", "Knight D Pelegrini", true},
	};

	for (auto& a : AnimPath)
	{
		AssetLoader::AnimationLoad(a.path, a.boneName, a.isLeftHand);
	};

	

	GameObject* gameObject = &ObjectFunc::CreateEmpty("Ground");
	CP_SpriteRenderer* pSpriteRenderer = gameObject->AddComponent<CP_SpriteRenderer>();
	pSpriteRenderer->SetTexture(*pTextures[0]);
	gameObject->transform.SetScale(Vector3::One * 50.0f);

	GameObject* player = &ObjectFunc::CreateEmpty("Player");
	player->AddComponent<CP_Player>();

	gameObject = &ObjectFunc::CreateEmpty("Sword");
	gameObject->transform.SetParent(player->transform);
	CP_Weapon* weapon = gameObject->AddComponent<CP_Weapon>();
	weapon->SetGrabBoneName("mixamorig:RightHand");
	
	CP_Animation* anim = player->GetComponent<CP_Animation>();
	Bone* bone = anim->GetSkeletalMesh().GetBoneList().FindBone(weapon->GetGrabBoneName());
	weapon->SetGrabBone(*bone);
	
	gameObject = &ObjectFunc::CreateEmpty("spot");
	gameObject->transform.SetParent(player->transform);
	CP_MeshRenderer* m = gameObject->AddComponent<CP_MeshRenderer>();
	m->SetRenderMesh("spot");

	/*gameObject = &ObjectFunc::CreateEmpty("spot");
	m = gameObject->AddComponent<CP_MeshRenderer>();
	m->SetRenderMesh("spot.fbx");*/

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
