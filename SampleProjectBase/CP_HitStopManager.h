#pragma once
#include "SingletonComponent.h"
#include "HitStopper_Base.h"

class CP_HitStopManager : public SingletonComponent<CP_HitStopManager>
{
private:
	friend class SingletonComponent<CP_HitStopManager>;

	/// @brief �o�^����Ă���q�b�g�X�g�b�v
	std::list<HitStopper_Base*> hitStoppers;

	/// @brief �~�߂�t���[����
	u_int stopFrame { 0 };

	/// @brief ���݂̎~�߂Ă���t���[����
	u_int curStopFrame { 0 };

	/// @brief �q�b�g�X�g�b�v�����H
	bool isHitStopping { false };
public:
	/// @brief �q�b�g�X�g�b�p�[��ǉ�����
	/// @param _addHitStopper �ǉ�����q�b�g�X�g�b�p�[
	void AddHitStopper(HitStopper_Base& _addHitStopper);

	/// @brief �q�b�g�X�g�b�p�[���폜����
	/// @param _removeHitStopper �폜����q�b�g�X�g�b�p�[
	void RemoveHitStopper(HitStopper_Base& _removeHitStopper);

	/// @brief �q�b�g�X�g�b�v�J�n
	/// @param _stopFrame �~�߂�t���[����
	void HitStopBegin(u_int _stopFrame);
private:
	//CP_HitStopManager();
	/*~CP_HitStopManager() {}*/

	void Update() override;

	/// @brief �q�b�g�X�g�b�v���̍X�V����
	void HitStopUpdate();

	/// @brief �q�b�g�X�g�b�v�I�����̏���
	void OnHitStopEnd();
};

