#include "pch.h"
#include "Tank_InGameSub.h"

Tank_InGameSub::Tank_InGameSub(SceneMoveInfo* _moveInfo) : SubScene_Base(_moveInfo)
{
	Model* model = nullptr;
	ModelSettings modelSetting;
	modelSetting.Setup("assets/model/tank/Tank02.fbx", "Spot",false);
	ModelLoader::Load(modelSetting);
	
	std::unique_ptr<Object_3D> object = std::make_unique<Object_3D>();
	model = resourceCollection->GetResource<Model>("Md_Spot");
	object->SetModel(model);
	
	/*object->GetModel().SetPixelShader(ShaderCollection::GetInstance()->GetPixelShader("PS_VertexColor"));*/
	sceneObjects->SetObject("Spot", std::move(object));
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
