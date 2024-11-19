#include "pch.h"
#include "CP_CameraMove.h"
#include "GameObject.h"

#include "CP_Camera.h"

#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;


CP_CameraMove::CP_CameraMove() :
	pTargetTransform(nullptr), pCamera(nullptr)
{
}

void CP_CameraMove::Init()
{
	// コントローラを作成
	pMoveController = std::make_unique<CameraMoveController>();
}

bool CP_CameraMove::IsCanUpdate()
{
#ifdef EDIT
	if (pTargetTransform == nullptr) return false;
#endif // EDIT

	return true;
}

void CP_CameraMove::Start()
{
	// カメラを取得する
	pCamera = gameObject->GetComponent<CP_Camera>();
	if (pCamera == nullptr)
		assert(!"Cameraコンポーネントをつけてください");

	pMoveController->Begin(*pCamera);	// 初期処理
	pMoveController->SetTargetTransform(pTargetTransform);
}

void CP_CameraMove::LateUpdate()
{
	if (!IsCanUpdate()) return;

	// カメラ移動を更新
	pMoveController->UpdateCall();
}

void CP_CameraMove::SetTargetTransform(const Transform* _targetTransform)
{
	pTargetTransform = _targetTransform;
	pMoveController->SetTargetTransform(_targetTransform);
}

void CP_CameraMove::ImGuiDebug()
{
	ImGuiSetTarget();

	// コントローラの編集
	pMoveController->ImGuiCall();
}

void CP_CameraMove::ImGuiSetTarget()
{
	// ターゲットの名前を探してセットする
	std::string text = "target:";

	if (pTargetTransform)
		text += pTargetTransform->GetObjectName();

	ImGui::Text(text.c_str());
	static char str[IM_INPUT_BUF];
	ImGui::InputText("targetName", str, IM_INPUT_BUF);

	if (ImGui::Button("Set"))
	{
		SceneObjects& objects = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* go = objects.GetSceneObject(str);
		if (go)
			pTargetTransform = &go->GetConstTransform();
	}
}

nlohmann::json CP_CameraMove::Save()
{
	auto data = Component::Save();

	// ターゲットのオブジェクトをセーブ
	if (pTargetTransform)
		data["target"] = pTargetTransform->GetObjectName();

	// 移動コントローラのセーブ
	data["moveController"] = pMoveController->Save();

	return data;
}

void CP_CameraMove::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	// ターゲット名からオブジェクト取得
	std::string targetObjName;
	LoadJsonString("target", targetObjName, _data);

	// シーンないから探す
	SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
	GameObject* pTargetObject = sceneObjs.GetSceneObject(targetObjName);
	if (!pTargetObject) return;

	// 対象トランスフォームにセット
	pTargetTransform = &pTargetObject->GetConstTransform();

	// コントローラのロード
	nlohmann::json controllerData;
	if (HashiTaku::LoadJsonData("moveController", controllerData, _data))
	{
		pMoveController->Load(controllerData);
	}
}
