#include "Test_SubScene.h"
#include "Triangle.h"
#include "Direct3D11.h"
#include "GameObject.h"
#include "ModelCollect.h"

Test_SubScene::Test_SubScene(SceneMoveInfo* _pSceneMoveInfo)
	: SubScene_Base(_pSceneMoveInfo)
{
	ModelCollect* MC = ModelCollect::GetInstance();

	ModelSettings set;
	set.Setup("assets/spot.fbx", "Cow", true);
	ModelCollect::GetInstance()->Load(set);

	go = new GameObject();
	go->SetModel(MC->GetConstResource(set.modelName));
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
