#pragma once
#include "CP_Button.h"
#include "CP_UIRenderer.h"

/// @brief �{�^�����O���[�v�����āA�Ǘ�����
class CP_ButtonGroup : public Component
{
	/// @brief �Z���N�g�C���[�W�̍��W��
	std::vector<DirectX::SimpleMath::Vector3> selectTargetPos;

	/// @brief �f�t�H���g�̃{�^����ID
	u_int defaultSelectButtonId;
protected:
	/// @brief �{�^���̃O���[�v
	std::vector<CP_Button*> buttonGroup;

	/// @brief �{�^���̃R���|�[�l���g�����I�u�W�F�N�g
	std::vector<std::string> buttonObjNames;

	/// @brief �Z���N�g���Ă鎞�̉摜
	CP_UIRenderer* pSelectBackImage;

	/// @brief �Z���N�g���Ă鎞�̉摜�I�u�W�F�N�g��
	std::string backImageObjName;

	/// @brief ���ݑI��ł���{�^����ID
	int curSelectButtonId;

	/// @brief �{�^���̐�
	int maxButtonCnt;

	/// @brief �c���͂ňړ�����{�^���̐�
	int vertMoveSpeed;

	/// @brief �����͂ňړ�����{�^���̐�
	int horiMoveSpeed;

	/// @brief �ړ����邱�Ƃ��ł��邩
	bool canInput;
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

	void Update() override;

	/// @brief ���͂őI���{�^�����ړ�����
	void MoveButton();

	/// @brief �{�^��������
	void DecideButton();

	/// @brief �Z���N�g�̔w�i�C���[�W��I�𒆂̃{�^���ֈړ�
	void MoveSelectBackImage();

	/// @brief �ő吔���Z�b�g
	/// @param _max �ő吔
	void SetMaxButtonCnt(int _max);

	void ImGuiDebug() override;
};

