#include "Test_SubScene.h"
#include "Triangle.h"
#include "Direct3D11.h"

#include "Model.h"

Test_SubScene::Test_SubScene(SceneMoveInfo* _pSceneMoveInfo)
	: SubScene_Base(_pSceneMoveInfo)
{
	ModelSettings settings;
	settings.modelPath = "assets/Alicia_solid_Unity.FBX";
	settings.isRighthand = true;
	cube = new Model(settings);
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
	cube->Draw();
}

void Test_SubScene::Release()
{
	CLASS_DELETE(cube);
}
