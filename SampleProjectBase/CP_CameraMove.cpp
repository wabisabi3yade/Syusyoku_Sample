#include "pch.h"
#include "CP_CameraMove.h"
#include "GameObject.h"

#include "CP_Camera.h"

#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

void CP_CameraMove::UpdateVector()
{
	Transform& transform = GetTransform();

	rotateVec = GameInput::GetInstance()->GetValue(GameInput::ValueType::Camera_Move).x;
}

void CP_CameraMove::Move()
{
	centerAngle += rotateVec * rotateSpeed * MainApplication::DeltaTime();
	Mathf::Repeat(centerAngle, Mathf::roundDeg);
	
	Transform& transform = GetTransform();
	Vector3 position = transform.GetPosition();
	Vector3 targetPos = pTargetObj->GetTransform().GetPosition();

	float centerRad = centerAngle * Mathf::degToRad;

	position.x = targetPos.x + cos(centerRad) * distanceHori;
	position.z = targetPos.z + sin(centerRad) * distanceHori;
	position.y = targetPos.y + distanceVer;

	transform.SetPosition(position);
}

void CP_CameraMove::LookUpdate()
{
	Vector3 targetPos = pTargetObj->GetTransform().GetPosition();
	targetPos += offsetTarget;

	GetTransform().LookAt(targetPos);
}

bool CP_CameraMove::IsCanUpdate()
{
#ifdef EDIT
	if (pTargetObj == nullptr) return false;
#endif // EDIT

	return true;
}


CP_CameraMove::CP_CameraMove()
	:pTargetObj(nullptr), pCamera(nullptr), rotateSpeed(120.f), centerAngle(0.0f), distanceHori(3.0f), distanceVer(1.5f)
{
}

void CP_CameraMove::Start()
{
	pCamera = gameObject->GetComponent<CP_Camera>();	// カメラを取得する

	if (pCamera == nullptr)
		HASHI_DEBUG_LOG("Cameraコンポーネントがありません");
}

void CP_CameraMove::LateUpdate()
{
	if (!IsCanUpdate()) return;

	UpdateVector();
	Move();
	LookUpdate();
}

void CP_CameraMove::ImGuiSetting()
{
	ImGui::DragFloat("rotSpeed", &rotateSpeed);
	ImGuiMethod::DragFloat3(offsetTarget, "offsetTarget", 0.1f);
	ImGui::DragFloat("dis_Hori", &distanceHori, 0.1f);
	ImGui::DragFloat("dis_Veri", &distanceVer, 0.1f);

	std::string text = "target ";
	if (pTargetObj)
		text += pTargetObj->GetName();

	ImGui::Text(text.c_str());
	static char str[IM_INPUT_BUF];
	ImGui::InputText("targetName", str, IM_INPUT_BUF);

	if (ImGui::Button("Set"))
	{
		SceneObjects& objects = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* go = objects.GetSceneObject(str);
		if (go)
			pTargetObj = go;
	}

}

nlohmann::json CP_CameraMove::Save()
{
	auto data = Component::Save();

	if (pTargetObj)
		data["target"] = pTargetObj->GetName();

	data["rotSpeed"] = rotateSpeed;
	SaveJsonVector3("offsetTarget", offsetTarget, data);
	data["dis_Hori"] = distanceHori;
	data["dis_Ver"] = distanceVer;

	return data;
}

void CP_CameraMove::Load(const nlohmann::json& _data)
{
	// ターゲット名からオブジェクト取得
	std::string targetObjName;
	LoadJsonString("target", targetObjName, _data);
	SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
	pTargetObj = sceneObjs.GetSceneObject(targetObjName);

	LoadJsonVector3("offsetTarget", offsetTarget, _data);
	LoadJsonFloat("rotSpeed", rotateSpeed, _data);
	LoadJsonFloat("dis_Hori", distanceHori, _data);
	LoadJsonFloat("dis_Ver", distanceVer, _data);
}
