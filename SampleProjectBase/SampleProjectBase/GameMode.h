#pragma once

class Camera;
class GameObject;

// �Q�[���Ŏg�p����J�����A�v���C���[�Ȃǂ̏����擾����
class GameMode :  public Singleton_Base<GameMode>
{
	friend class Singleton_Base<GameMode>;

	Camera* pMainCamera;	// �V�[�����̃��C���J����
	std::vector<GameObject*> pPlayers;	// �v���C���[�z��

public:
	Camera& GetCamera() { return *pMainCamera; }
	GameObject* GetPlayer(u_int _idx);

	void SetCamera(Camera& _camera);
	void SetPleyer(GameObject& _player, u_int _idx);
};

