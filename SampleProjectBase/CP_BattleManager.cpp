#include "pch.h"
#include "CP_BattleManager.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"
#include "Geometory.h"
#include "CP_CameraMove.h"
#include "SceneManager.h"

namespace HashiTaku
{
	void CP_BattleManager::Init()
	{
		SingletonComponent::Init();

		// オブザーバーを作成
		pDirectionEndObserver = std::make_unique<BattleDirectionEndObserver>();

		// 演出作成
		pBattleDirection = std::make_unique<BattleDirection>();
		// オブザーバー追加
		pBattleDirection->AddObserver(*pDirectionEndObserver);
	}

	void CP_BattleManager::SetPlayer(CP_Player& _playerObj)
	{
		pPlayer = &_playerObj;
	}

	void CP_BattleManager::AddEnemy(CP_Enemy& _enemyObj)
	{
		auto itr = std::find(enemyList.begin(), enemyList.end(), &_enemyObj);

		if (itr != enemyList.end()) return;

		enemyList.push_back(&_enemyObj);
	}

	void CP_BattleManager::RemovePlayer(CP_Player& _playerObj)
	{
		if (pPlayer != &_playerObj) return;

		pPlayer = nullptr;
	}

	void CP_BattleManager::RemoveEnemy(CP_Enemy& _enemyObj)
	{
		enemyList.remove(&_enemyObj);
	}

	CP_Player* CP_BattleManager::GetPlayerObject()
	{
		return pPlayer;
	}

	const CP_BattleManager::EnemyList& CP_BattleManager::GetEnemyList()
	{
		return enemyList;
	}
	void CP_BattleManager::OnPlayerWin()
	{
		// オブジェクトを止める
		StopObjects();
		if (static_cast<u_int>(enemyList.size()) == 0) return;	// 敵がいないなら
		Transform& bossTransform = (*enemyList.begin())->GetTransform();

		// 勝利演出を開始する
		pBattleDirection->OnBeginWinDirection(&bossTransform);
	}

	void CP_BattleManager::OnPlayerLose()
	{
		// オブジェクトを止める
		StopObjects();
		if (static_cast<u_int>(enemyList.size()) == 0) return;	// 敵がいないなら
		Transform& bossTransform = (*enemyList.begin())->GetTransform();

		// 敗北演出
		pBattleDirection->OnBeginLoseDirection(&bossTransform);
	}

	json CP_BattleManager::Save()
	{
		auto data = SingletonComponent::Save();

		SaveJsonVector4("moveAreaRect", moveAreaRect, data);
		data["battleDirection"] = pBattleDirection->Save();

		return data;
	}

	void CP_BattleManager::Load(const json& _data)
	{
		SingletonComponent::Load(_data);

		LoadJsonVector4("moveAreaRect", moveAreaRect, _data);

		json loadData;
		if (LoadJsonData("battleDirection", loadData, _data))
		{
			pBattleDirection->Load(loadData);
		}
	}

	void CP_BattleManager::Awake()
	{

		SingletonComponent::Awake();

		// カメラ移動クラスを取得
		CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();
		pCamMove = camera.GetGameObject().GetComponent<CP_CameraMove>();

		// バトル演出の初期処理
		pBattleDirection->Init(pCamMove);
	}

	void CP_BattleManager::Update()
	{
		pBattleDirection->Update(DeltaTime());

		if (!isDirectionStart)
			FadeStart();
	}

	void CP_BattleManager::LateUpdate()
	{
		// 移動範囲制限
		MoveAreaUpdate();
	}

	void CP_BattleManager::Draw()
	{
#ifdef EDIT
		if (!isDebugDisplay) return;

		constexpr float height = 0.3f;
		DXSimp::Color c = DXSimp::Color(1, 0, 0);

		DXSimp::Vector3 RT = { moveAreaRect.x, height, moveAreaRect.z };
		DXSimp::Vector3 RB = { moveAreaRect.x, height, moveAreaRect.w };
		DXSimp::Vector3 LT = { moveAreaRect.y, height, moveAreaRect.z };
		DXSimp::Vector3 LB = { moveAreaRect.y, height, moveAreaRect.w };

		Geometory::AddLine(RT, RB, c);
		Geometory::AddLine(RB, LB, c);
		Geometory::AddLine(LB, LT, c);
		Geometory::AddLine(LT, RT, c);
#endif // EDIT
	}

	void CP_BattleManager::FadeStart()
	{
		isDirectionStart = true;

		// オブジェクトを動けないようにする
		StopObjects();

		// ゲームスタートの演出
		pBattleDirection->OnBeginStartDirection();
	}

	void CP_BattleManager::OnBeginBattle()
	{
		// 活動状態にする
		ActiveObjects();
	}

	void CP_BattleManager::EndBattle()
	{
		// タイトルシーンに移動
		SceneManager::GetInstance()->ChangeSceneRequest("Title");
	}

	void CP_BattleManager::MoveAreaUpdate()
	{
		// 移動範囲内に制限
		if (pPlayer)
			PositionClamp(pPlayer->GetTransform());

		for (auto& enemy : enemyList)
		{
			PositionClamp(enemy->GetTransform());
		}
	}

	void CP_BattleManager::PositionClamp(Transform& _charaTransform)
	{
		//　y座標を考慮しない距離を求める
		DXSimp::Vector3 pos = _charaTransform.GetPosition();

		pos.x = std::clamp(pos.x, moveAreaRect.y, moveAreaRect.x);
		pos.z = std::clamp(pos.z, moveAreaRect.w, moveAreaRect.z);

		_charaTransform.SetPosition(pos);
	}

	void CP_BattleManager::ActiveObjects()
	{
		// 各オブジェクトのアクティブ状態に
		if (pPlayer)
			pPlayer->SetEnable(true);

		for (auto& enemy : enemyList)
			enemy->SetEnable(true);

		if (pCamMove)
			pCamMove->SetEnable(true);
	}

	void CP_BattleManager::StopObjects()
	{
		// 各オブジェクトの非アクティブ状態に
		if (pPlayer)
			pPlayer->SetEnable(false);

		for (auto& enemy : enemyList)
			enemy->SetEnable(false);

		if (pCamMove)
			pCamMove->SetEnable(false);
	}

	void CP_BattleManager::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Checkbox("IsDisplay", &isDebugDisplay);
		ImGui::DragFloat4("AreaRect", &moveAreaRect.x, 0.01f);

		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Direction");
		pBattleDirection->ImGuiCall();
#endif // EDIT
	}

	BattleDirectionEndObserver::BattleDirectionEndObserver() :
		IObserver("BattleDirectionEndObserver")
	{
		pBattleManager = CP_BattleManager::GetInstance();
	}

	void BattleDirectionEndObserver::ObserverUpdate(const int& _value)
	{
		BattleDirection::DirectionState dirState =
			static_cast<BattleDirection::DirectionState>(_value);

		// 状態によって行う処理を変える
		switch (dirState)
		{
		case BattleDirection::DirectionState::Start:
			pBattleManager->OnBeginBattle();
			break;

		case BattleDirection::DirectionState::Win:
			pBattleManager->EndBattle();
			break;

		case BattleDirection::DirectionState::Lose:
			pBattleManager->EndBattle();
			break;

		default:
			break;
		}

	}
}