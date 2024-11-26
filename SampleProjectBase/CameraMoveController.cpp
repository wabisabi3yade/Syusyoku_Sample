#include "pch.h"
#include "CameraMoveController.h"
#include "GameObject.h"

#include "CameraOnMoveState.h"
#include "CameraTargetMove.h"

namespace DXSimp = DirectX::SimpleMath;

// 前回の揺れの時間の初期値
constexpr float INIT_PREV_SHAKEELAPSETIME(-10000.0f);

CameraMoveController::CameraMoveController() :
	StateMachine_Base("CameraMove"), pTargetTransform(nullptr), pLookAtTransform(nullptr),
	shakeTime(0.0f), shakeElapsedTime(0.0f), prevShakeElapsedTime(0.0f), isShaking(false)
{
	using enum CameraMoveState_Base::CameraState;

	// ステートを作成
	CreateState<CameraOnMoveState>(Move);
	CreateState<CameraTargetMove>(Target);

	// デフォルト状態をセット
	SetDefaultNode(Move);
}

void CameraMoveController::Begin(CP_Camera& _camera)
{
	pCamera = &_camera;

	StateMachine_Base::Begin();
}

void CameraMoveController::Update()
{
	// ステートからカメラのベース座標を更新
	StateMachine_Base::Update();

	//TestShakeUpdate();

	// 揺れの大きさをオブジェクト側の座標に反映
	ShakeUpdate();

	// 最終的なカメラの座標を更新
	UpdateFinalPos();
}

void CameraMoveController::ShakeUpdate()
{
	if (!isShaking) return;

	// 揺らす間隔を取得
	ShakeLevelParam& param = shakeLevelParameter[static_cast<u_int>(curShakeLevel)];

	/// 経過時間を加算
	shakeElapsedTime += GetCamera().DeltaTime();

	// 全体時間を過ぎたら終わる
	if (shakeElapsedTime > shakeTime)
		OnEndShake();

	// 前回揺らしてから間隔分時間が空いたら
	else if (shakeElapsedTime - prevShakeElapsedTime > param.duration)
	{
		// 揺れのオフセット値を更新
		curShakeOffset = CalcShakeOffset(curShakeLevel, curShakeType);
		prevShakeElapsedTime = shakeElapsedTime;
	}
}

void CameraMoveController::UpdateFinalPos()
{
	Transform& camTransform = pCamera->GetTransform();

	// カメラの向きを考慮したワールド空間の揺れを求める
	DXSimp::Vector3 worldShakeOffset;
	if (isShaking)
	{
		worldShakeOffset = camTransform.Right() * curShakeOffset.x +
			camTransform.Up() * curShakeOffset.y +
			camTransform.Forward() * curShakeOffset.z;
	}

	// 座標をセット
	camTransform.SetPosition(curBaseCameraPos + worldShakeOffset);
}

void CameraMoveController::ChangeState(CameraMoveState_Base::CameraState _cameraState, bool _isForce)
{
	ChangeNode(_cameraState, _isForce);
}

void CameraMoveController::SetFollowTransform(const Transform* _pTransform)
{
	pTargetTransform = _pTransform;
}

void CameraMoveController::SetLookAtObject(const ITargetAccepter* _pTransform)
{
	pLookAtTransform = _pTransform;
}

void CameraMoveController::SetCameraBasePos(const DirectX::SimpleMath::Vector3& _camPos)
{
	curBaseCameraPos = _camPos;
}

void CameraMoveController::SetFov(float _setDegree)
{
	pCamera->SetFov(_setDegree);
}

void CameraMoveController::BeginShake(ShakeLevel _level, ShakeType _type, float _time)
{
	// 前揺れているのがあるなら上書きする
	// パラメータのリセット
	curShakeLevel = _level;
	curShakeType = _type;
	shakeTime = _time;
	shakeElapsedTime = 0.0f;
	prevShakeElapsedTime = INIT_PREV_SHAKEELAPSETIME;

	// 揺れを開始
	isShaking = true;
}

float CameraMoveController::GetFov() const
{
#ifdef EDIT
	if (!pCamera) return 0.0f;
#endif // EDIT

	return pCamera->GetFov();
}

CP_Camera& CameraMoveController::GetCamera()
{
	return *pCamera;
}

const DirectX::SimpleMath::Vector3& CameraMoveController::GetCameraBasePos()
{
	return curBaseCameraPos;
}

const Transform* CameraMoveController::GetFollowTransform() const
{
	return pTargetTransform;
}

bool CameraMoveController::GetHaveTarget() const
{
	return pTargetTransform != nullptr;
}

const DirectX::SimpleMath::Vector3& CameraMoveController::GetLookAtWorldPos() const
{
	if (!pLookAtTransform) return DirectX::SimpleMath::Vector3::Zero;

	return pLookAtTransform->GetWorldPosByTargetObj();
}

nlohmann::json CameraMoveController::Save()
{
	nlohmann::json data;

	// 各状態をセーブ
	auto& stateDatas = data["stateDatas"];
	for (auto& state : stateNodeList)
	{
		nlohmann::json stateData;

		CameraMoveState_Base& camState = CastCamState(*state.second);

		stateData["stateType"] = state.first;	// ステート名
		stateData["parameter"] = camState.Save();	// ステートのセーブ情報
		stateDatas.push_back(stateData);	// 追加
	}

	// 揺れレベル
	auto& shakeDatas = data["shakeLevels"];
	for (auto& level : shakeLevelParameter)
	{
		nlohmann::json levelData;

		levelData["duration"] = level.duration;
		levelData["power"] = level.power;

		shakeDatas.push_back(levelData);
	}


	return data;
}

void CameraMoveController::Load(const nlohmann::json& _data)
{
	LoadStates(_data);

	// レベルごとの揺れのパラメータ
	nlohmann::json shakeDatas;
	if (HashiTaku::LoadJsonDataArray("shakeLevels", shakeDatas, _data))
	{
		u_int levelId = 0;
		for (auto& shData : shakeDatas)
		{
			// 配列の範囲外指定するなら
			if (levelId >= static_cast<u_int>(ShakeLevel::MaxNum)) break;

			auto& shLvParam = shakeLevelParameter[levelId];
			HashiTaku::LoadJsonFloat("duration", shLvParam.duration, shData);
			HashiTaku::LoadJsonFloat("power", shLvParam.power, shData);
			levelId++;
		}
	}
}

void CameraMoveController::OnEndShake()
{
	isShaking = false;
	curShakeOffset = DXSimp::Vector3::Zero;
}

std::string CameraMoveController::GetStateName(CameraMoveState_Base::CameraState _state)
{
	return 	std::string(
		magic_enum::enum_name<CameraMoveState_Base::CameraState>(_state)
	);
}

DirectX::SimpleMath::Vector3 CameraMoveController::CalcShakeOffset(ShakeLevel _level, ShakeType _type)
{
	DXSimp::Vector3 offset;

	// 揺らす大きさ取得
	u_int levelId = static_cast<u_int>(curShakeLevel);
	ShakeLevelParam& param = shakeLevelParameter[levelId];

	// 乱数で揺れのオフセット値を取得
	switch (_type)
	{
	case ShakeType::Vertical:
		offset.y = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		break;

	case ShakeType::Horizon:
		offset.x = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		break;

	case ShakeType::Both:
		offset.x = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		offset.y = HashiTaku::Random::Range<float>(-1.0f * param.power, param.power);
		break;
	}

	return offset;
}

CameraMoveState_Base& CameraMoveController::CastCamState(HashiTaku::StateNode_Base& _stateBase)
{
	return static_cast<CameraMoveState_Base&>(_stateBase);
}

void CameraMoveController::TestShakeUpdate()
{
	const GameKey& key = MainApplication::GetInput().GetKeyboard();
	if (key.GetKeyDown(DIK_H))
		BeginShake(testLevel, testType, testTime);
	if (key.GetKeyDown(DIK_J))
	{
		u_int shakeId = static_cast<u_int>(testLevel);
		shakeId++;
		if (shakeId >= static_cast<u_int>(ShakeLevel::MaxNum))
		{
			shakeId = 0;
		}

		testLevel = static_cast<ShakeLevel>(shakeId);
	}
	if (key.GetKeyDown(DIK_K))
	{
		u_int shakeId = static_cast<u_int>(testType);
		shakeId++;
		if (shakeId >= static_cast<u_int>(ShakeType::MaxNum))
		{
			shakeId = 0;
		}

		testType = static_cast<ShakeType>(shakeId);
	}
}

void CameraMoveController::ImGuiDebug()
{
	// 現在のステート名
	std::string curStateName = "current" + GetStateName(currentStateKey);
	ImGui::Text(curStateName.c_str());

	// 各ステート編集
	for (auto& state : stateNodeList)
	{
		// ステート名取得
		std::string stateName = GetStateName(state.first);

		if (ImGuiMethod::TreeNode(stateName))
		{
			CameraMoveState_Base& camState =
				static_cast<CameraMoveState_Base&>(*state.second);
			camState.ImGuiCall();
			ImGui::TreePop();
		}
	}

	ImGuiShakeLevel();
}

void CameraMoveController::ImGuiShakeLevel()
{
#ifdef EDIT
	if (!ImGuiMethod::TreeNode("ShakeLevel")) return;

	std::string s = "testLevel:" + std::string(magic_enum::enum_name<ShakeLevel>(testLevel));
	ImGui::Text(s.c_str());
	std::string s1 = "testType:" + std::string(magic_enum::enum_name<ShakeType>(testType));
	ImGui::Text(s1.c_str());
	ImGui::DragFloat("testTime", &testTime);

	// 各揺れの強さを設定
	u_int levelCnt = static_cast<u_int>(shakeLevelParameter.size());
	for (u_int l_i = 0; l_i < levelCnt; l_i++)
	{
		std::string levelName =
			std::string(magic_enum::enum_name<ShakeLevel>(static_cast<ShakeLevel>(l_i)));

		if (ImGuiMethod::TreeNode(levelName))
		{
			ImGui::DragFloat("duration", &shakeLevelParameter[l_i].duration, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("power", &shakeLevelParameter[l_i].power, 0.01f, 0.0f, 100.0f);

			ImGui::TreePop();
		}
	}

	ImGui::TreePop();
#endif // EDIT
}

void CameraMoveController::LoadStates(const nlohmann::json& _data)
{
	nlohmann::json stateDatas;
	// ステートのデータがないなら
	if (!HashiTaku::LoadJsonDataArray("stateDatas", stateDatas, _data)) return;

	// 各状態をロード
	for (auto& stateData : stateDatas)
	{
		// ステートのタイプをロード
		CameraMoveState_Base::CameraState loadState;
		if (!HashiTaku::LoadJsonEnum<CameraMoveState_Base::CameraState>(
			"stateType", loadState, stateData
		)) continue;

		// 作成されていないなら
		if (!stateNodeList.contains(loadState)) continue;

		// パラメータのデータがないなら
		nlohmann::json paramData;
		if (!HashiTaku::LoadJsonData("parameter", paramData, stateData))
			continue;

		// ロードする
		CastCamState(*stateNodeList[loadState]).Load(paramData);
	}
}

