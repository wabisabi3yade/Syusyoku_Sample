#include "pch.h"
#include "CP_CameraMove.h"
#include "GameObject.h"

#include "CP_Camera.h"

#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;


CP_CameraMove::CP_CameraMove() :
	pFollowTransform(nullptr), pCamera(nullptr), pTargetObject(nullptr)
{
}

CP_CameraMove::~CP_CameraMove()
{
	if (pTargetObject)
		pTargetObject->RemoveTargeter(*this);

	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->RemoveCamMove(*this);
	}
}

void CP_CameraMove::Init()
{
	// コントローラを作成
	pMoveController = std::make_unique<CameraMoveController>();
}

bool CP_CameraMove::IsCanUpdate()
{
#ifdef EDIT
	if (pFollowTransform == nullptr) return false;
#endif // EDIT

	return true;
}

void CP_CameraMove::FindFollowObject()
{
	// シーン内から探す
	SceneObjects& objects =
		InSceneSystemManager::GetInstance()->GetSceneObjects();

	GameObject* go = objects.GetSceneObject(followObjName);

	if (go)	// あったらトランスフォームを取得
		pFollowTransform = &go->GetConstTransform();
}

void CP_CameraMove::Awake()
{
	if (CP_BattleManager* pBattle = CP_BattleManager::GetInstance())
	{
		pBattle->SetCameraMove(*this);
	}
}

void CP_CameraMove::Start()
{
	// 追従先のオブジェクトを探す
	FindFollowObject();

	// カメラを取得する
	pCamera = gameObject->GetComponent<CP_Camera>();
	if (pCamera == nullptr)
		assert(!"Cameraコンポーネントをつけてください");

	pMoveController->SetFollowTransform(pFollowTransform);
	pMoveController->Begin(*pCamera);	// 初期処理
}

void CP_CameraMove::LateUpdate()
{
	if (!IsCanUpdate()) return;

	// カメラ移動を更新
	pMoveController->UpdateCall();
}

void CP_CameraMove::SetFollowTransform(const Transform* _followTransform)
{
	pFollowTransform = _followTransform;
	pMoveController->SetFollowTransform(_followTransform);
}

void CP_CameraMove::SetTargetObject(ITargetAccepter& _targetObject)
{
	// ターゲットオブジェクトを取得
	pTargetObject = &_targetObject;
	pTargetObject->SetTargeter(*this);
	pMoveController->SetLookAtObject(&_targetObject);
}

void CP_CameraMove::RemoveNotify(const ITargetAccepter& _removeObj)
{
	// ターゲット側から解除してきたなら
	if (pTargetObject != &_removeObj) return;

	pTargetObject = nullptr;
	pMoveController->SetLookAtObject(nullptr);
}

void CP_CameraMove::OnPlayerWin(const Transform& _targetTransform)
{
	pMoveController->OnPlayerWin(_targetTransform);
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
	std::string text = "follow:" + followObjName;
	ImGui::Text(text.c_str());

	// 入力で変更
	static char str[IM_INPUT_BUF];
	ImGui::InputText("objName", str, IM_INPUT_BUF);
	if (ImGui::Button("Set Follow"))
	{
		followObjName = str;
	}
}

nlohmann::json CP_CameraMove::Save()
{
	auto data = Component::Save();

	// ターゲットのオブジェクトをセーブ
	data["target"] = followObjName;

	// 移動コントローラのセーブ
	data["moveController"] = pMoveController->Save();

	return data;
}

void CP_CameraMove::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	// ターゲット名からオブジェクト取得
	LoadJsonString("target", followObjName, _data);

	// コントローラのロード
	nlohmann::json controllerData;
	if (HashiTaku::LoadJsonData("moveController", controllerData, _data))
	{
		pMoveController->Load(controllerData);
	}
}
