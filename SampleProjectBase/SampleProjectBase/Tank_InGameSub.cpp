#include "pch.h"
#include "Tank_InGameSub.h"
#include "PlayerMove.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CameraMove.h"
#include "CameraInput.h"
#include "Field.h"
#include "ShaderSphere.h"
#include "SinCurve.h"

constexpr u_int SPHERE_NUM(2);
constexpr u_int CUBE_NUM(2);

Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	std::vector<ModelSettings> modelSets =
	{
	};
	// ロード処理をする
	for (auto m : modelSets)
	{
		ModelLoader::Load(m);
	}
	std::vector<TextureLoader::TextureSetting> textureSets =
	{
		{"assets/texture/Grass.png", "T_Grass01"}
	};
	Model* model = nullptr;
	Texture* tex = nullptr;

	// カメラ
	GameObject* camera = sceneObjects->GetSceneObject<GameObject>("MainCamera");
	CameraMove* camMove = camera->AddComponent<CameraMove>();
	camera->AddComponent<CameraInput>();

	// 地面
	std::unique_ptr<Field> field = std::make_unique<Field>();
	tex = TextureLoader::Load(textureSets[0]);
	field->SetTexture(tex);
	sceneObjects->SetObject("Field", std::move(field));

	//　ディレクションライト
	std::unique_ptr<DirectionLight> pDirLight = std::make_unique<DirectionLight>();
	sceneLights->SetDirectionLight(std::move(pDirLight));

	ShaderCollection* shCollect = ShaderCollection::GetInstance();
	VertexShader* vs = shCollect->GetVertexShader("VS_Gouraud");
	PixelShader* ps = shCollect->GetPixelShader("PS_Unlit");

	// 球を作成
	std::vector<std::unique_ptr<ShaderSphere>> spheres;
	for (u_int sphereLoop = 0; sphereLoop < SPHERE_NUM; sphereLoop++)
	{
		std::unique_ptr<ShaderSphere> createSphere = std::make_unique<ShaderSphere>();
		spheres.push_back(std::move(createSphere));
	}
	// unlitマテリアルを作成
	std::unique_ptr<MaterialClass> unlitMaterial = std::make_unique<MaterialClass>();
	unlitMaterial->SetVertexShader(vs);
	unlitMaterial->SetPixelShader(ps);
	spheres[0]->SetMaterial(*unlitMaterial.get());
	resourceCollection->SetResource<MaterialClass>("UnlitMaterial", std::move(unlitMaterial));
	spheres[0]->AddComponent<SinCurve>();
	spheres[0]->AddComponent<SphereCollider>();
	sceneObjects->SetObject("UnlitSphere", std::move(spheres[0]));

	// 同じ名前があるなら名前の後に数字を入れる
	spheres[1]->AddComponent<SphereCollider>();
	sceneObjects->SetObject("UnlitSphere", std::move(spheres[1]));

	//// キューブを作成
	//std::vector<std::unique_ptr<Object_3D>> spheres;
	//for (u_int sphereLoop = 0; sphereLoop < SPHERE_NUM; sphereLoop++)
	//{
	//	std::unique_ptr<ShaderSphere> createSphere = std::make_unique<ShaderSphere>();
	//	spheres.push_back(std::move(createSphere));
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
