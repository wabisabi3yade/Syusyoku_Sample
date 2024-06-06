#include "Test_SubScene.h"
#include "Triangle.h"
#include "Direct3D11.h"
#include "GameObject.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;
Test_SubScene::Test_SubScene(SceneMoveInfo* _pSceneMoveInfo)
	: SubScene_Base(_pSceneMoveInfo)
{
	ModelSettings set;
	set.Setup("assets/spot.fbx", "Cow", true);

	ModelLoader::Load(set);

	go = new GameObject();
	Model* m = resourceCollection->GetResource<Model>("Md_Cow");
	go->SetModel(m);
}

Test_SubScene::~Test_SubScene()
{
	Release();
}

void Test_SubScene::Update()
{
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
	ImGuiIO& io = ImGui::GetIO(); 
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

void Test_SubScene::LateUpdate()
{
	mainCamera->Update();
}

void Test_SubScene::Draw()
{
	go->Draw();
	
}

void Test_SubScene::Release()
{
	CLASS_DELETE(go);
}
