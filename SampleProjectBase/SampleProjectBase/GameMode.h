#pragma once

class Camera;
class GameObject;

// ゲームで使用するカメラ、プレイヤーなどの情報を取得する
class GameMode :  public Singleton_Base<GameMode>
{
	friend class Singleton_Base<GameMode>;

	Camera* pMainCamera;	// シーン内のメインカメラ
	std::vector<GameObject*> pPlayers;	// プレイヤー配列

public:
	Camera& GetCamera() { return *pMainCamera; }
	GameObject* GetPlayer(u_int _idx);

	void SetCamera(Camera& _camera);
	void SetPleyer(GameObject& _player, u_int _idx);
};

