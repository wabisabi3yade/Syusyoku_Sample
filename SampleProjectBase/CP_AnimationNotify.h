#pragma once
#include "Component.h"
#include "AnimationNotify_Base.h"

#ifdef EDIT
#include "AnimationNotifyFactory.h"
#endif // EDIT

class AnimationNode_Base;
class CP_Animation;
class AnimationController;

/// @brief �A�j���[�V�����̒ʒm�C�x���g�R���|�[�l���g
class CP_AnimationNotify : public Component
{
	/// @brief �ʒm�C�x���g�ƑΏۂƂȂ�A�j���[�V�����m�[�h
	struct NotifyGroup
	{
		const AnimationNode_Base* pAnimNode;
		std::list<std::unique_ptr<AnimationNotify_Base>> pNotifys;
	};

	/// @brief �ʒm�C�x���g�O���[�v���X�g
	std::list<std::unique_ptr<NotifyGroup>> notifyGroups;

	/// @brief ���݂̒ʒm�O���[�v
	NotifyGroup* pCurrentGroup;

	/// @brief �A�j���[�V�����R���|�[�l���g
	AnimationController* pAnimController;	

	/// @brief ���݂̃A�j���[�V��������
	float curRatio;

	/// @brief �Ō�ɌĂяo���ꂽ�Ƃ��̊���
	float lastRatio;

	/// @brief ���[�v�������H
	bool isLoop;

#ifdef EDIT
	std::unique_ptr<AnimationNotifyFactory> pFactory;
#endif // EDIT

public:
	CP_AnimationNotify();
	~CP_AnimationNotify() {}

	void ImGuiSetting() override;

private:
	void Init() override;
	void Start() override;
	void LateUpdate() override;

	/// @brief �X�V�ł��邩�m�F
	/// @return �X�V�ł��邩�H
	bool IsCanUpdate();

	/// @brief �������X�V����
	void UpdateRatio();

	/// @brief �C�x���g�ʒm���X�V����
	void NotifyUpdate();
	
	/// @brief ���݂̃A�j���[�V�����ƃC�x���g�ʒm�𓯊�������
	void LinkCurGroup();
	
	/// @brief �R���g���[���[�ƃm�[�h���X�V����
	void UpdateCurGroup();

	/// @brief ����ׂ̈̑O�񊄍����X�V����
	void UpdateLastRatio();

	/// @brief Animtion�ɐV�K�m�[�h���ǉ����ꂽ�Ƃ��ɂ������ɂ��Ή�����
	void UpdateNotifyGroups();

	/// @brief �C�x���g�ʒm���쐬����
	/// @param _addGroup �ǉ�����O���[�v
	/// @param _typeName �쐬����C�x���g�ʒm�̌^��
	void CreateNotify(NotifyGroup& _addGroup, const std::string& _typeName);

	/// @brief �A�j���[�V�����R���g���[���[�擾
	AnimationController* GetAnimController();

	// �O���[�v���Ƃ̕ҏW
	void ImGuiGroupSetting();
};