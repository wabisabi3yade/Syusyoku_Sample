#pragma once
#include "PlayerActionController_Base.h"
#include "PlayerAirState.h"

/// @brief �v���C���[�̋󒆍s�����p�����Ă��邩�R���Z�v�g
template<typename T>
concept PlayerAirConcept =
std::is_base_of_v<PlayerAirState, T>;

/// @brief �v���C���[�󒆃A�N�V�����R���g���[���[
class PlayerAirActionController : public PlayerActionController_Base
{
public:
	using AirState = PlayerAirState::PlayerState;

private:
	/// @brief ���ɗ͂������邩�H
	bool isDownForce;

	/// @brief �������ɂ�����́i�d�͂��ƁA�������̑傫�����j
	float downForcePower;

public:
	/// @brief �R���X�g���N�^
	/// @param _pAction �A�N�V����
	/// @param _player �v���C���[
	PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player);
	~PlayerAirActionController() {}

	/// @brief �X�V����
	void Update() override;

	/// @brief �s����Ԃ�؂�ւ���
	/// @param _nextActionName ���̏�Ԃ̖��O
	/// @param _isForce �����Ő؂�ւ����邩�H
	bool ChangeAirState(const  AirState& _nextActionState, bool _isForce = false);

	/// @brief ���ɗ͂������邩�Z�b�g����
	/// @param _isDown �͂������邩�H
	void SetIsDownForce(bool _isDown);

	/// @brief �eState�̕�������擾����
	/// @param _stateId ��Ԃ�ID
	/// @return ������
	std::string GetStateStr(int _stateId) override;

	/// @brief �eState��ID���擾����
	/// @param _stateName�@��Ԗ�
	/// @return ��Ԃ�ID
	int GetStateId(const std::string& _stateName) override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	template<PlayerAirConcept T>
	void CreateState(AirState _actionState);

	/// @brief ���z�̗����ɂ��邽�߂ɉ������̗͂�������
	void AddDownForce();

	void ImGuiDebug() override;
};

template<PlayerAirConcept T>
inline void PlayerAirActionController::CreateState(AirState _actionState)
{
	PlayerActionController_Base::CreateState<T>(static_cast<int>(_actionState));
}