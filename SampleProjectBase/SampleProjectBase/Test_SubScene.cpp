#include "Test_SubScene.h"
#include "Triangle.h"
#include "Direct3D11.h"

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
	go->SetModel(m);
	VertexShader* vertexSh = ShaderCollection::GetInstance()->GetVertexShader("VertexShader");
	PixelShader* pixelSh = ShaderCollection::GetInstance()->GetPixelShader("PixelShader");

	go->GetModel().SetVertexShader(vertexSh);
	go->GetModel().SetPixelShader(pixelSh);
	sceneObjects->SetObject("Cow", std::move(go));


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


	sceneObjects->Draw();
}

void Test_SubScene::Release()
{
}
