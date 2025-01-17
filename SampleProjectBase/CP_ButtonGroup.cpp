#include "pch.h"
#include "CP_ButtonGroup.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	CP_ButtonGroup::CP_ButtonGroup() :
		pSelectBackImage(nullptr),
		curSelectButtonId(0),
		defaultSelectButtonId(0),
		maxButtonCnt(0),
		vertMoveSpeed(0),
		horiMoveSpeed(0),
		canInput(true)
	{
	}

	void CP_ButtonGroup::SetCanMove(bool _canMove)
	{
		canInput = _canMove;
	}

	void CP_ButtonGroup::OpenDisplay()
	{
		canInput = true;
		curSelectButtonId = 0;	// �����ʒu�Ɉړ�

		// �{�^����\��
		SetDisplayButtons(true);

		// �{�^���̍��W��ύX����
		SettingButtonPosition();

		// �I���C���[�W���ړ�
		MoveSelectBackImage();
	}

	void CP_ButtonGroup::CloseDisplay()
	{
		// ���͕s�ɂ���
		canInput = false;

		// �{�^�������\��������
		SetDisplayButtons(false);
	}

	json CP_ButtonGroup::Save()
	{
		auto data = Component::Save();

		data["defaultId"] = defaultSelectButtonId;
		data["vertSpeed"] = vertMoveSpeed;
		data["horiSpeed"] = horiMoveSpeed;
		data["buttonObjNames"] = buttonObjNames;
		data["backObjName"] = backImageObjName;

		auto& targetData = data["buttonParameter"];
		for (int b_i = 0; b_i < maxButtonCnt; b_i++)
		{
			json posData;
			SaveJsonVector3("buttonPos", buttonParameterGroup[b_i].buttonPos, posData);
			SaveJsonVector3("selectOffset", selectOffsetPosFromButton[b_i], posData);
			targetData.push_back(posData);
		}

		return data;
	}

	void CP_ButtonGroup::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonUnsigned("defaultId", defaultSelectButtonId, _data);
		LoadJsonInteger("vertSpeed", vertMoveSpeed, _data);
		LoadJsonInteger("horiSpeed", horiMoveSpeed, _data);
		LoadJsonString("backObjName", backImageObjName, _data);

		if (IsJsonContains(_data, "buttonObjNames"))
		{
			const json& namesData = _data["buttonObjNames"];
			int idx = 0;
			for (auto& nameData : namesData)
			{
				if (idx >= maxButtonCnt) break;
				buttonObjNames[idx] = nameData.get<std::string>();
				idx++;
			}
		}

		// ���W
		json targetData;
		if (LoadJsonDataArray("buttonParameter", targetData, _data))
		{
			int i = 0;
			for (auto& posData : targetData)
			{
				LoadJsonVector3("buttonPos", buttonParameterGroup[i].buttonPos, posData);
				LoadJsonVector3("selectOffset", selectOffsetPosFromButton[i], posData);
				i++;
			}
		}
	}

	void CP_ButtonGroup::Start()
	{
		if (static_cast<u_int>(buttonParameterGroup.size()) < defaultSelectButtonId)
		{
			HASHI_DEBUG_LOG("�f�t�H���g��ID���{�^���̐��𒴂��Ă��܂�");
			defaultSelectButtonId = 0;
		}
		if (static_cast<u_int>(buttonParameterGroup.size()) == 0)
		{
			HASHI_DEBUG_LOG("�{�^����1������܂���");
		}

		// �f�t�H���g����n�߂�
		curSelectButtonId = defaultSelectButtonId;


		// �e�R���|�[�l���g���擾
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* pGO = sceneObjs.GetSceneObject(backImageObjName);
		if (pGO)
		{
			pSelectBackImage = pGO->GetComponent<CP_UIRenderer>();
		}

		for (int b_i = 0; b_i < maxButtonCnt; b_i++)
		{
			// �ݒ肵���{�^���̖��O����{�^���R���|�[�l���g���擾
			pGO = sceneObjs.GetSceneObject(buttonObjNames[b_i]);
			if (!pGO) continue;
			CP_Button* pGetButton = pGO->GetComponent<CP_Button>();
			buttonParameterGroup[b_i].pButton = pGetButton;

			if (!pGetButton)	// �{�^���擾�o���Ȃ�������
				HASHI_DEBUG_LOG(buttonObjNames[b_i] + "����{�^�����擾�ł��܂���ł���");
		}
	}

	void CP_ButtonGroup::Update()
	{
		if (maxButtonCnt == 0) return;	// �{�^����1���ݒ肳��Ă��Ȃ��Ȃ�
		if (!canInput) return;	// ����ł��Ȃ��Ȃ甲����

		MoveButton();
		DecideButton();
	}

	void CP_ButtonGroup::MoveButton()
	{
		const InputClass& gameInput = MainApplication::GetInput();
		u_int prevSelectId = curSelectButtonId;	// �ύX�O�̃{�^����Id

		int inputX = 0, inputY = 0;

		// ���͂ɂ���ă{�^���̈ړ����s��
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Up))
		{
			inputY += 1;
		}
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Down))
		{
			inputY -= 1;
		}
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Right))
		{
			inputX += 1;
		}
		if (gameInput.GetInputTriggerDir(InputClass::InputDirection::Left))
		{
			inputX -= 1;
		}

		// �I�𒆂̃{�^�����ړ�
		curSelectButtonId += inputX * horiMoveSpeed + inputY * vertMoveSpeed;
		// �O�ƕς���Ă��Ȃ��Ȃ珈�����Ȃ�
		if (curSelectButtonId == prevSelectId) return;

		// �ő吔�𒴂�����
		if (curSelectButtonId >= maxButtonCnt)
		{
			curSelectButtonId = 0;
		}
		else if (curSelectButtonId < 0) // 0���������
		{
			curSelectButtonId = maxButtonCnt - 1;
		}

		MoveSelectBackImage();	// �I���̔w�i�C���[�W���ړ�������
	}

	void CP_ButtonGroup::DecideButton()
	{
		const InputClass& gameInput = MainApplication::GetInput();
		const GamePad& gamePad = gameInput.GetGamePad();

		bool isDecide = false;
		if (gamePad.GetConnecting()) // �R���g���[��
		{
			isDecide = gamePad.InputTrigger(GamePad::PadFlag::Batsu);
		}
		else // �L�[�{�[�h
		{

			isDecide = gameInput.GetKeyboard().GetKeyDown(DIK_RETURN);
		}

		// ���肵���炻�̃{�^���̃C�x���g�𔭐�������
		if (isDecide && buttonParameterGroup[curSelectButtonId].pButton)
		{
			buttonParameterGroup[curSelectButtonId].pButton->OnEvent();
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

		// �{�^������̃I�t�Z�b�g���W�ɑI���C���[�W���ړ�������
		DXSimp::Vector3 buttonPos = buttonParameterGroup[curSelectButtonId].buttonPos + 
			selectOffsetPosFromButton[curSelectButtonId];

		// Z���W�ȊO�𔽉f������
		buttonPos.z = backImageTrans.GetPosition().z;
		backImageTrans.SetPosition(buttonPos);
	}

	void CP_ButtonGroup::SettingButtonPosition()
	{
		// �{�^���̃I�u�W�F�N�g���W��ݒ艺���W�ֈړ�
		for (u_int b_i = 0; b_i < maxButtonCnt; b_i++)
		{
			CP_Button* pButton = buttonParameterGroup[b_i].pButton;
			if (!pButton) continue;
			
			// �ړ�������
			pButton->GetTransform().SetPosition(buttonParameterGroup[b_i].buttonPos);
		}
	}

	void CP_ButtonGroup::SetDisplayButtons(bool _isDisplay)
	{
		// �{�^��
		for (auto& buttonParam : buttonParameterGroup)
		{
			buttonParam.pButton->GetGameObject().SetActive(_isDisplay);
		}

		// �I���C���[�W
		if (pSelectBackImage)
			pSelectBackImage->GetGameObject().SetActive(_isDisplay);
	}

	void CP_ButtonGroup::SetMaxButtonCnt(int _max)
	{
		maxButtonCnt = _max;
		buttonParameterGroup.resize(maxButtonCnt);
		buttonObjNames.resize(maxButtonCnt);
		selectOffsetPosFromButton.resize(maxButtonCnt);
	}

	void CP_ButtonGroup::ImGuiDebug()
	{
#ifdef EDIT
		ImGuiMethod::Text("CanInput", canInput);
		ImGui::DragInt("VertSpeed", &vertMoveSpeed);
		ImGui::DragInt("HoriSpeed", &horiMoveSpeed);

		// �I�u�W�F�N�g���Z�b�g
		static char inputText[IM_INPUT_BUF];
		ImGui::InputText("objName", inputText, IM_INPUT_BUF);
		ImGui::Text("Back:%s", backImageObjName.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Set"))
			backImageObjName = inputText;

		for (int b_i = 0; b_i < maxButtonCnt; b_i++)
		{
			ImGui::PushID(b_i);

			ImGui::Text("%d:%s", b_i, buttonObjNames[b_i].c_str());
			ImGui::SameLine();
			if (ImGui::Button("Set"))
				buttonObjNames[b_i] = inputText;

			// �{�^�����W
			ImGui::DragFloat3("ButtonPos", &buttonParameterGroup[b_i].buttonPos.x);
			// �Z���N�g�̈ʒu
			ImGui::DragFloat3("SelectOffset", &selectOffsetPosFromButton[b_i].x);

			ImGui::PopID();
		}
#endif // EDIT
	}
}