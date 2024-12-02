#pragma once
#include "CP_Button.h"
#include "CP_UIRenderer.h"

/// @brief �{�^�����O���[�v�����āA�Ǘ�����
class CP_ButtonGroup : public Component
{
	/// @brief �f�t�H���g�̃{�^����ID
	u_int defaultSelectButtonId;

	/// @brief �ړ����邱�Ƃ��ł��邩
	bool canSelectMove;
protected:
	/// @brief �{�^���̃O���[�v
	std::vector<CP_Button*> buttonGroup;

	/// @brief �{�^���̃R���|�[�l���g�����I�u�W�F�N�g
	std::vector<std::string> buttonObjNames;

	/// @brief �Z���N�g���Ă鎞�̃{�^���̃C���[�W
	CP_UIRenderer* pSelectBackImage;

	/// @brief ���ݑI��ł���{�^����ID
	u_int curSelectButtonId;

	/// @brief �c���͂ňړ�����{�^���̐�
	int vertMoveSpeed;

	/// @brief �����͂ňړ�����{�^���̐�
	int horiMoveSpeed;
public:
	CP_ButtonGroup();
	virtual ~CP_ButtonGroup() {}

	/// @brief �ړ��ł��邩�Z�b�g����
	/// @param _canMove �ړ��ł��邩�t���O
	void SetCanMove(bool _canMove);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	void Start() override;

	/// @brief ���͂őI���{�^�����ړ�����
	void MoveButton();

	/// @brief �Z���N�g�̔w�i�C���[�W��I�𒆂̃{�^���ֈړ�
	void MoveSelectBackImage();

	void ImGuiDebug() override;
};

