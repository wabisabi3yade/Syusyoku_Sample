#pragma once

class CP_Camera;
class GameObject;

// ゲームで使用するカメラ、プレイヤーなどの情報を取得する
class GameMode :  public Singleton_Base<GameMode>
{
	friend class Singleton_Base<GameMode>;

	CP_Camera* pMainCamera;	// シーン内のメインカメラ
	std::vector<GameObject*> pPlayers;	// プレイヤー配列

	GameMode() : pMainCamera(nullptr){}
	~GameMode() {};
public:
	CP_Camera& GetCamera() { return *pMainCamera; }
	GameObject* GetPlayer(u_int _idx);

	void SetCamera(CP_Camera& _camera);
	void SetPleyer(GameObject& _player, u_int _idx);
};

