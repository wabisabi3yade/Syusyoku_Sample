#pragma once
#include "Singleton_Base.h"

class BroadScene_Base;
class ChangeBroadScene;
class SceneMoveInfo;

class SceneManager :public Singleton_Base<SceneManager>
{
	friend class Singleton_Base<SceneManager>;

	static SceneManager* pInstance;	// �C���X�^���X
	BroadScene_Base* pNowBroadScene = nullptr;	// �����s���Ă����ǃV�[��
	ChangeBroadScene* pChaneBroad = nullptr;	// ��ǃV�[����J�ڂ���N���X
	SceneMoveInfo* pMoveInfo;	// �V�[���J�ڏ��

	SceneManager();
	~SceneManager();

	/// @brief �A�Z�b�g�̏���
	void AssetSetup();

	void CheckChangeBroad();	// �V�[����J�ڂ��邩�m�F����

	// �������
	void Release();

public:

	// ���s�֐�
	void Exec();

private:
	/// @brief �}�e���A���̏���
	void MaterialSetup();

	/// @brief ���b�V���̏���
	void BasicMeshSetup();
};

