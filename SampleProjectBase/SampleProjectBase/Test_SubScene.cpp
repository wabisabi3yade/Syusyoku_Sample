#include "Test_SubScene.h"
#include "Triangle.h"
#include "Direct3D11.h"
#include "GameObject.h"


Test_SubScene::Test_SubScene(SceneMoveInfo* _pSceneMoveInfo)
	: SubScene_Base(_pSceneMoveInfo)
{
	ModelSettings set;
	set.Setup("assets/spot.fbx", "Cow", true);

	ModelLoader::Load(set);

	go = new GameObject();
	Model* m = resourceCollection->GetResource<Model>("M_Cow");
	go->SetModel(m);
}

Test_SubScene::~Test_SubScene()
{
	Release();
}

void Test_SubScene::Update()
{
	
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
