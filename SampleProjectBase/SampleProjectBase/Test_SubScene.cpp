#include "Test_SubScene.h"
#include "Triangle.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "ImGuiMethod.h"
using namespace DirectX::SimpleMath;
Test_SubScene::Test_SubScene(SceneMoveInfo* _pSceneMoveInfo)
	: SubScene_Base(_pSceneMoveInfo)
{
	ModelSettings set;
	set.Setup("assets/spot.fbx", "Cow", true);
	ModelLoader::Load(set);

	Model* m = resourceCollection->GetResource<Model>("Md_Cow");
	std::unique_ptr<Object_3D> go = std::make_unique<Object_3D>();
	std::unique_ptr<Object_3D> go2 = std::make_unique<Object_3D>();
	go->SetModel(m);
	go2->SetModel(m);
	go2->transform.position.x = 3.f;
	VertexShader* vertexSh = ShaderCollection::GetInstance()->GetVertexShader("VertexShader");
	PixelShader* pixelSh = ShaderCollection::GetInstance()->GetPixelShader("PixelShader");

	sceneObjects->SetObject("Cow", std::move(go));
	sceneObjects->SetObject("Cow", std::move(go2));

	cube = std::make_unique<Cube>();
}

Test_SubScene::~Test_SubScene()
{
	Release();
}

void Test_SubScene::Update()
{
	sceneObjects->Update();
}

void Test_SubScene::LateUpdate()
{
	sceneObjects->LateUpdate();
}

void Test_SubScene::Draw()
{
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	Shader* shader[] =
	{
		shCol->GetVertexShader("VertexShader"),
		shCol->GetPixelShader("PixelShader"),
	};
	Object_3D* go = sceneObjects->GetSceneObject<Object_3D>("Cow");
	ImGui::Begin("a");
	ImGui::SliderFloat("posX", &go->transform.position.z, -10, 10);
	ImGui::End();

	cube->Draw();

	go->GetModel().SetVertexShader(shader[0]);
	go->GetModel().SetPixelShader(shader[1]);
	sceneObjects->Draw();

	
}

void Test_SubScene::Release()
{
}
