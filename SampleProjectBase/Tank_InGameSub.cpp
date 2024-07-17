#include "pch.h"
#include "Tank_InGameSub.h"

// コンポーネント
#include "ComponentDefine.h"

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

	std::vector<Mesh_Base*> pModels;

	struct ModelData
	{
		std::string path;
		float scale;
		bool isLeftHand;
		bool isGetScale;
	};

	std::vector<ModelData> SMPaths =
	{
		{"assets/model/spot/spot.fbx", 1.0f, true},
		{"assets/model/knight/Knight D Pelegrini.fbx", 0.01f, false},
	};

	Mesh_Base* pSM = AssetLoader::ModelLoad(SMPaths[1].path, SMPaths[1].scale, SMPaths[1].isLeftHand);
	pModels.push_back(pSM);

	GameObject* gameObject = &Object::CreateEmpty("Ground");
	CP_SpriteRenderer* pSpriteRenderer = gameObject->AddComponent<CP_SpriteRenderer>();
	pSpriteRenderer->SetTexture(*pTextures[0]);
	gameObject->transform.scale = Vector3::One * 50.0f;

	gameObject = &Object::CreateEmpty("knight");
	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh(*pModels[0]);
	gameObject->AddComponent<CP_BoxCollider>();


	/*Model* model = nullptr;
	Texture* tex = nullptr;*/

	//SceneObjects& sceneObjects = systemManager->GetSceneObjects();
	//SceneLights& sceneLights = systemManager->GetSceneLights();

	//// カメラ
	//GameObject* camera = sceneObjects.GetSceneObject<GameObject>("MainCamera");
	//CameraMove* camMove = camera->AddComponent<CameraMove>();
	//camera->AddComponent<CameraInput>();

	//// 地面
	//std::unique_ptr<Field> field = std::make_unique<Field>();
	//texLoader.Load(texturePath[0]);
	//CP_SpriteRenderer* spriteRenderer = field->GetComponent<CP_SpriteRenderer>();
	//spriteRenderer->SetTexture(*tex);
	//sceneObjects.SetObject("Field", std::move(field));

	////　ディレクションライト
	//std::unique_ptr<DirectionLight> pDirLight = std::make_unique<DirectionLight>();
	//sceneLights.SetDirectionLight(std::move(pDirLight));


	//// unlitマテリアルを作成
	//ShaderCollection* shCollect = ShaderCollection::GetInstance();
	//VertexShader* vs = shCollect->GetVertexShader("VS_Gouraud");
	//PixelShader* ps = shCollect->GetPixelShader("PS_Unlit");
	//Material* setMaterial = nullptr;	
	//std::unique_ptr<Material> unlitMaterial = std::make_unique<Material>();
	//unlitMaterial->SetVertexShader(vs);
	//unlitMaterial->SetPixelShader(ps);
	//setMaterial = resourceCollection->SetResource<Material>("UnlitMaterial", std::move(unlitMaterial));

	//Mesh* sm_sphere = resourceCollection->GetResource<Mesh>("SM_Sphere");

	//// 球を作成
	//for (u_int sphereLoop = 0; sphereLoop < SPHERE_NUM; sphereLoop++)
	//{
	//	std::unique_ptr<GameObject> createSphere = std::make_unique<GameObject>();
	//	CP_MeshRenderer* m = createSphere->AddComponent<CP_MeshRenderer>();
	//	createSphere->AddComponent<CP_SphereCollider>();
	//	RenderMesh mesh(sm_sphere, 0);
	//	m->SetRenderMesh(mesh);

	//	sceneObjects.SetObject("unlitObj", std::move(createSphere));
	//}
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
