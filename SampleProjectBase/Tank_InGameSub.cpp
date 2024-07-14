#include "pch.h"
#include "Tank_InGameSub.h"

#include "Field.h"

// コンポーネント
#include "ComponentDefine.h"

constexpr u_int SPHERE_NUM(1);
constexpr u_int CUBE_NUM(2);

using namespace SceneFunction;
namespace fs = std::filesystem;

Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	std::vector<Texture*> pTextures;

	std::vector<std::string> texturePath =
	{
		{"assets/texture/an.jpg"}
	};
	for (auto texPath : texturePath)
	{
		Texture* pTex = AssetLoader::TextureLoad(texPath);
		pTextures.push_back(pTex);
	}

	std::vector<Mesh_Base*> pModels;
	std::vector<std::string> SMPaths =
	{
		{"assets/model/spot/spot.fbx"}
	};
	for (auto modelPath : SMPaths)
	{
		fs::path path = modelPath;
		Mesh_Base* pSM = AssetLoader::ModelLoad(modelPath, 1.0f, true);
		pModels.push_back(pSM);
	}

	GameObject* gameObject = &Object::CreateEmpty("Ground");
	CP_SpriteRenderer* pSpriteRenderer = gameObject->AddComponent<CP_SpriteRenderer>();
	pSpriteRenderer->SetTexture(*pTextures[0]);

	gameObject = &Object::CreateEmpty("Cube");
	CP_MeshRenderer* pMeshRenderer = gameObject->AddComponent<CP_MeshRenderer>();
	pMeshRenderer->SetRenderMesh(*pModels[0]);

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
