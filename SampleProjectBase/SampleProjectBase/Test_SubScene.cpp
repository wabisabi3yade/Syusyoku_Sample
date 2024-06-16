#include "Test_SubScene.h"

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

	std::unique_ptr<Object_3D> Cow = std::make_unique<Object_3D>();
	Cow->SetModel(m);
	Cow->GetModel().SetVertexShader(ShaderCollection::GetInstance()->GetVertexShader("VertexShader"));
	Cow->GetModel().SetPixelShader(ShaderCollection::GetInstance()->GetPixelShader("PixelShader"));
	/*sceneObjects->SetObject("Cow", std::move(Cow))*/;

	cube = std::make_unique<Cube>();

	std::unique_ptr<Texture> texPtr = std::make_unique<Texture>();
	texPtr->Load("assets/texture/field000.jpg");
	Texture* tex = texPtr.get();
	resourceCollection->SetResource<Texture>("T_field01", std::move(texPtr));

	std::unique_ptr<Object_2D> field = std::make_unique<Object_2D>();
	field->SetTexture(tex);
	sceneObjects->SetObject("field", std::move(field));

	std::unique_ptr<Texture> texPtr2 = std::make_unique<Texture>();
	texPtr2->Load("assets/texture/dora01.png");
	tex = texPtr2.get();
	resourceCollection->SetResource<Texture>("T_Doragon01", std::move(texPtr2));

	std::unique_ptr<Object_UI> doragon = std::make_unique<Object_UI>();
	doragon->SetTexture(tex);
	sceneObjects->SetObject("doragon", std::move(doragon));
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
	Camera* cam = sceneObjects->GetSceneObject<Camera>("mainCamera");
	
	static float angleSpeed = 0.0f;
	static float angle = 0;
	angle += angleSpeed;
	ImGui::Begin("Cube");
	ImGui::DragFloat("rotSpeed", &angleSpeed);
	
	static Transform t = {};
	
	ImGuiMethod::DragFloat3(t.position, "Cubepos");
	ImGui::End();
	
	cube->SetRotation(Vector3(angle, 0, 0));
	cube->SetPosition(t.position);
	cube->Draw();

	sceneObjects->Draw();
}

void Test_SubScene::Release()
{
}
