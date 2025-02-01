#include "pch.h"
#include "PlayerGuardState.h"
#include "CP_Player.h"
#include "CP_SoundManager.h"

namespace HashiTaku
{
	constexpr auto GUARDEND_ANIM("Guard_End");
	constexpr auto GUARDKNOCK_ANIM("Guard_Knock");
	constexpr auto PARRYTRIGGER_NAME("parryTrigger");

	PlayerGuardState::PlayerGuardState() :
		canParry(false), 
		sustainParryFrame(4),
		parryElapsedFrame(0),
		parryAddGuardGage(25.0f),
		canParryForwardAngle(180.0f),
		addStylishPointOnParry(200.0f)
	{
	}

	bool PlayerGuardState::GetCanParry(const DXSimp::Vector3& _enemyPos)
	{
		if (!canParry) return false;

		// �U���𐳖ʂŎ󂯂�Ă��邩�m�F(�G�Ƃ̍��W)
		// �v���C���[�̐��ʂɓG�����邩�m�F
		Transform& transform = GetMyTransform();
		DXSimp::Vector3 toEnemyVec = _enemyPos - transform.GetPosition();
		toEnemyVec.y = 0.0f; toEnemyVec.Normalize();
		float dot = toEnemyVec.Dot(transform.Forward());

		if (dot < cos(canParryForwardAngle * Mathf::degToRad * 0.5f)) return false;

		return true;
	}

	void PlayerGuardState::OnParry()
	{
		// �p���B�g���K�[��true
		GetAnimation()->SetTrigger(PARRYTRIGGER_NAME);

		// �A�j���[�V����
		GetAnimation()->SetBool(GUARD_PARAMNAME, false);

		// �L�����Z���S�Ăł���悤�ɂ���
	/*	GetAnimation()->SetBool(CANCEL_PARAMNAME, true);
		GetAnimation()->SetBool(CANATK_PARAMNAME, false);*/

		// �G�t�F�N�g���o��
		CreateParryVfx();

		// ���ʉ���炷
		PlayParrySE();

		// �X�^�C���b�V���|�C���g�����Z
		GetPlayer().AddStylishPoint(addStylishPointOnParry);

		// �O���͂���Ă�����
		if (IsInputVector(InputVector::Forward))
			ReleaseAttack();	// �U���ɔh��
	}

	json PlayerGuardState::Save()
	{
		auto data = PlayerGroundState::Save();
		data["canParryTime"] = sustainParryFrame;
		data["parryAngle"] = canParryForwardAngle;
		SaveJsonVector3("vfxOffset", createVfxOffset, data);
		data["vfxInfo"] = parryEffectInfo.Save();
		data["addStylishPointParry"] = addStylishPointOnParry;
		for (auto& itr : parrySoundParameters)
		{
			data["parrySEs"].push_back(itr.Save());
		}

		return data;
	}

	void PlayerGuardState::Load(const json& _data)
	{
		PlayerGroundState::Load(_data);
		LoadJsonUnsigned("canParryTime", sustainParryFrame, _data);
		LoadJsonFloat("parryAngle", canParryForwardAngle, _data);
		LoadJsonVector3("vfxOffset", createVfxOffset, _data);
		LoadJsonFloat("addStylishPointParry", addStylishPointOnParry, _data);
		json loadData;
		if (LoadJsonData("vfxInfo", loadData, _data))
		{
			parryEffectInfo.Load(loadData);
		}

		if (LoadJsonDataArray("parrySEs", loadData, _data))
		{
			for (auto& soundData : loadData)
			{
				PlaySoundParameter loadSound;
				loadSound.Load(soundData);
				parrySoundParameters.push_back(loadSound);
			}
		}
	}

	void PlayerGuardState::OnStartBehavior()
	{
		// �p���B�ł����Ԃɂ���
		canParry = true;
		parryElapsedFrame = 0;

		// �A�j���[�V����
		GetAnimation()->SetBool(GUARD_PARAMNAME, true);
	}

	void PlayerGuardState::UpdateBehavior()
	{
		// �p���B�ł��鎞�Ԃ̍X�V
		ParryTimeUpdate();

		CP_RigidBody& rb = GetRB();
		DXSimp::Vector3 velocity;
		velocity.y = rb.GetVelocity().y;
		GetRB().SetVelocity(velocity);
	}

	void PlayerGuardState::OnEndBehavior()
	{
		// �p���B�ł��Ȃ�����
		canParry = false;

		// �A�j���[�V�����ɃK�[�h�I���ʒm
		GetAnimation()->SetBool(GUARD_PARAMNAME, false);
	}

	void PlayerGuardState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{
		// �K�[�h�I���A�j���[�V�������I����
		if (_fromAnimNodeName == GUARDEND_ANIM || _fromAnimNodeName == GUARDKNOCK_ANIM)
			ChangeState(PlayerState::Idle);
	}

	void PlayerGuardState::ParryTimeUpdate()
	{
		if (!canParry) return;

		// �i�߂�
		parryElapsedFrame++;

		// �p���B�ł���t���[���łȂ��Ȃ�����p���B�ł��Ȃ���Ԃ�
		if (parryElapsedFrame > sustainParryFrame)
		{
			canParry = false;
			GetAnimation()->SetBool(GUARD_PARAMNAME, false);
		}
	}

	void PlayerGuardState::ReleaseAttack()
	{
		// �U���ɕύX
		ChangeState(PlayerState::SpecialAtkGuard);
	}

	void PlayerGuardState::CreateParryVfx()
	{
		Transform& transform = GetMyTransform();

		// �p���B�G�t�F�N�g���o���ꏊ�����߂�
		DXSimp::Vector3 createVfxPos = transform.GetPosition();
		createVfxPos += transform.Right() * createVfxOffset.x;
		createVfxPos += transform.Up() * createVfxOffset.y;
		createVfxPos += transform.Forward() * createVfxOffset.z;

		// �Đ�
		DX11EffecseerManager::GetInstance()->Play(parryEffectInfo,
			createVfxPos,
			transform.GetEularAngles());
	}

	void PlayerGuardState::PlayParrySE()
	{
		// �V�[�����ɃT�E���h�}�l�W���[�����邩�m�F
		CP_SoundManager* pSoundManager = CP_SoundManager::GetInstance();
		if (!pSoundManager) return;

		// �����_���ōĐ����鉹��ς���
		int soundCnt = static_cast<int>(parrySoundParameters.size());
		if (soundCnt == 0) return;

		int randId = Random::RangeInt<int>(0, soundCnt - 1);
		auto soundItr = parrySoundParameters.begin();
		soundItr = std::next(soundItr, randId);

		// �v���C���[����SE���Đ�
		pSoundManager->PlaySE(*soundItr, GetMyTransform().GetPosition());
	}

	void PlayerGuardState::ImGuiDebug()
	{
		PlayerGroundState::ImGuiDebug();

		// �p���B
		int imInt = static_cast<int>(sustainParryFrame);
		ImGui::DragInt("CanParry", &imInt, 1, 0, 100);
		sustainParryFrame = static_cast<u_int>(imInt);
		ImGui::DragFloat("ParryAngle", &canParryForwardAngle, 0.1f, 0.0f, 360.0f);
		ImGui::DragFloat("AddStylishPoint", &addStylishPointOnParry, 0.1f, 0.0f, 10000.0f);

		// �G�t�F�N�g
		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Parry Vfx");
		ImGui::DragFloat3("Offset", &createVfxOffset.x, 0.01f);
		parryEffectInfo.ImGuiCall();

		// �T�E���h
		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Parry SE");
		u_int loop = 1;
		for (auto itr = parrySoundParameters.begin(); itr != parrySoundParameters.end();)
		{
			bool isDelete = false;
			if (ImGuiMethod::TreeNode(std::to_string(loop)))
			{
				if (ImGui::Button("X"))
					isDelete = true;
				
				itr->ImGuiCall();

				ImGui::TreePop();
			}
			loop++;

			if (isDelete)
				itr = parrySoundParameters.erase(itr);
			else
				itr++;
		}
		if (ImGui::Button("+"))
		{
			PlaySoundParameter add;
			parrySoundParameters.push_back(add);
		}
	}
}