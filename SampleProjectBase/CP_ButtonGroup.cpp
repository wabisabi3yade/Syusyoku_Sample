#include "pch.h"
#include "CP_ButtonGroup.h"
#include "InSceneSystemManager.h"

/// @brief  �X�e�B�b�N�ňړ�����Ƃ��̓��̓f�b�h�]�[��
constexpr float STICK_DEADZONE(0.2f);

namespace DXSimp = DirectX::SimpleMath;

CP_ButtonGroup::CP_ButtonGroup() :
	pSelectBackImage(nullptr),
	curSelectButtonId(0),
	defaultSelectButtonId(0),
	vertMoveSpeed(0),
	horiMoveSpeed(0),
	canSelectMove(true)
{
}

void CP_ButtonGroup::SetCanMove(bool _canMove)
{
	canSelectMove = _canMove;
}

nlohmann::json CP_ButtonGroup::Save()
{
	auto data = Component::Save();

	data["defaultId"] = defaultSelectButtonId;
	data["vertSpeed"] = vertMoveSpeed;
	data["horiSpeed"] = horiMoveSpeed;

	return data;
}

void CP_ButtonGroup::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	HashiTaku::LoadJsonUnsigned("defaultId", defaultSelectButtonId, _data);
	HashiTaku::LoadJsonInteger("vertSpeed", vertMoveSpeed, _data);
	HashiTaku::LoadJsonInteger("horiSpeed", horiMoveSpeed, _data);
}

void CP_ButtonGroup::Start()
{
	if (static_cast<u_int>(buttonGroup.size()) < defaultSelectButtonId)
	{
		HASHI_DEBUG_LOG("�f�t�H���g��ID���{�^���̐��𒴂��Ă��܂�");
		defaultSelectButtonId = 0;
	}

	// �f�t�H���g����n�߂�
	curSelectButtonId = defaultSelectButtonId;
}

void CP_ButtonGroup::MoveButton()
{
	const GamePad& gameInput = MainApplication::GetInput().GetGamePad();
	DXSimp::Vector2 inputValue;

	if (gameInput.GetConnecting())
	{
		using enum GamePad::Value;
		using enum GamePad::Button;
		/*if(gameInput.GetValue(StickL_Y) > STICK_DEADZONE || gameInput.ButtonDown(Arrow_Up))
			inputValue.*/


	}
	else
	{

	}
}

void CP_ButtonGroup::MoveSelectBackImage()
{
	if (!pSelectBackImage)
	{
		HASHI_DEBUG_LOG("�I���C���[�W���ݒ肳��Ă��܂���");
		return;
	}

	// �I������Ă���{�^���̍��W���擾
	Transform& backImageTrans = pSelectBackImage->GetTransform();
	DXSimp::Vector3 buttonPos =
		buttonGroup[curSelectButtonId]->GetTransform().GetPosition();

	// Z���W�ȊO�𔽉f������
	buttonPos.z = backImageTrans.GetPosition().z;
	backImageTrans.SetPosition(buttonPos);
}

void CP_ButtonGroup::ImGuiDebug()
{
	ImGui::DragInt("VertSpeed", &vertMoveSpeed);
	ImGui::DragInt("HoriSpeed", &horiMoveSpeed);
}
