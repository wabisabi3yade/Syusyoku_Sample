#pragma once

class PlayerActionController;

class GameObject;

class PlayerState_Base
{
protected:
	PlayerActionController* pController;

	/// @brief  �v���C���[�̃g�����X�t�H�[��
	GameObject* pGameObject;

public:
	PlayerState_Base(PlayerActionController& _pController, GameObject& _gameObject) : pController(&_pController), pGameObject(&_gameObject) {}
	~PlayerState_Base() {}

	/// @brief ��Ԃ��ς�����Ƃ��̏�������
	virtual void Init() = 0;

	/// @brief �X�V����
	virtual void Update() = 0;

	/// @brief  �I������
	virtual void Terminal() = 0;
};

