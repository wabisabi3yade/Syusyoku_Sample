#include "pch.h"
#include "InputClass.h"

using namespace DirectX::SimpleMath;

InputClass::InputClass()
{
	gamePad = std::make_unique<GamePad>();
	keyboard = std::make_unique<GameKey>();

	inputValue.emplace("Left", Vector2::Zero);
	inputValue.emplace("Right", Vector2::Zero);
}

InputClass::~InputClass()
{
}

void InputClass::Init(HWND _hwnd)
{
	keyboard->Init(_hwnd);
}

void InputClass::Update()
{
	gamePad->InputUpdate();
	keyboard->InputUpdate();

	// �l���X�V����
	// ������
	Vector2 vecL;
	if (gamePad->GetValue(GamePad::Value::StickL_X) == 0.0f &&	// �R���g���[�����͂��Ȃ��Ȃ�
		gamePad->GetValue(GamePad::Value::StickL_Y) == 0.0f)
	{
		
		if (keyboard->GetKey(DIK_W))
		{
			vecL.y += 1.0f;
		}
		if ((keyboard->GetKey(DIK_S)))
		{
			vecL.y += -1.0f;
		}
		if ((keyboard->GetKey(DIK_D)))
		{
			vecL.x += 1.0f;
		}
		if ((keyboard->GetKey(DIK_A)))
		{
			vecL.x += -1.0f;
		}
	}
	else	// �R���g���[������
	{
		vecL.x = gamePad->GetValue(GamePad::Value::StickL_X);
		vecL.y = gamePad->GetValue(GamePad::Value::StickL_Y);
	}
	inputValue["Left"] = vecL;

	// ������
	Vector2 vecR;
	if (gamePad->GetValue(GamePad::Value::StickR_X) == 0.0f &&	// �R���g���[�����͂��Ȃ��Ȃ�
		gamePad->GetValue(GamePad::Value::StickR_Y) == 0.0f)
	{

		if (keyboard->GetKey(DIK_UP))
		{
			vecR.y += 1.0f;
		}
		if ((keyboard->GetKey(DIK_DOWN)))
		{
			vecR.y += -1.0f;
		}
		if ((keyboard->GetKey(DIK_RIGHT)))
		{
			vecR.x += 1.0f;
		}
		if ((keyboard->GetKey(DIK_LEFT)))
		{
			vecR.x += -1.0f;
		}
	}
	else	// �R���g���[������
	{
		vecR.x = gamePad->GetValue(GamePad::Value::StickR_X);
		vecR.y = gamePad->GetValue(GamePad::Value::StickR_Y);
	}
	inputValue["Right"] = vecR;
}

Vector2 InputClass::GetValue(const std::string& _getType) const
{
	if (!inputValue.contains(_getType))
	{
		ImGuiDebugLog::Add("���͒l�̎擾�����Ԉ���Ă��܂�");
		return Vector2::Zero;
	}

	return inputValue.at(_getType);
}
