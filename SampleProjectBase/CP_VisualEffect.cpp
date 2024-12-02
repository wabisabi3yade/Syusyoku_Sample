#include "pch.h"
#include "CP_VisualEffect.h"
#include "DX11EffecseerManager.h"
#include "AssetGetter.h"
#include "GameObject.h"

CP_VisualEffect::CP_VisualEffect() :
	pDxVfxManager(nullptr),
	pVFX(nullptr),
	vfxHandle(-1),
	currentPlayFrame(0.0f),
	playStartFrame(0),
	playEndFrame(0),
	playSpeed(1.0f),
	isPlaying(false),
	isLoop(false),
	isTrimming(false)
{
}

void CP_VisualEffect::Init()
{
	pDxVfxManager = DX11EffecseerManager::GetInstance();
}

void CP_VisualEffect::SetPlaySpeed(float _playSpeed)
{
	playSpeed = _playSpeed;
	pDxVfxManager->GetManager()->SetSpeed(vfxHandle, _playSpeed);	// ���x�ύX
}

void CP_VisualEffect::SetVisualEffect(const VisualEffect* _setVfx)
{
	pVFX = _setVfx;
}

nlohmann::json CP_VisualEffect::Save()
{
	auto data = Component::Save();

	data["isLoop"] = isLoop;
	data["playSpeed"] = playSpeed;
	data["isTrimming"] = isTrimming;
	if (isTrimming)
	{
		data["startFrame"] = playStartFrame;
		data["endFrame"] = playEndFrame;
	}

	if (pVFX)
		data["vfxName"] = pVFX->GetAssetName();

	return data;
}

void CP_VisualEffect::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	HashiTaku::LoadJsonBoolean("isLoop", isLoop, _data);
	HashiTaku::LoadJsonFloat("playSpeed", playSpeed, _data);
	HashiTaku::LoadJsonBoolean("isTrimming", isTrimming, _data);
	if (isTrimming)
	{
		HashiTaku::LoadJsonInteger("startFrame", playStartFrame, _data);
		HashiTaku::LoadJsonInteger("endFrame", playEndFrame, _data);
	}

	std::string vfxName;
	if (HashiTaku::LoadJsonString("vfxName", vfxName, _data))
	{
		pVFX = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}
}

void CP_VisualEffect::Start()
{
	// �Đ��J�n����
	BeginPlayVFX();
}

void CP_VisualEffect::Update()
{
	if (!isPlaying) return;

	TrimmingUpdate();

	PlayEndUpdate();
}

void CP_VisualEffect::TrimmingUpdate()
{
	if (!isTrimming) return;

	// �w�肵���I���t���[�����߂����
	if (static_cast<int>(currentPlayFrame) >= playEndFrame)
	{
		// �G�t�F�N�g���~�߂�
		pDxVfxManager->GetManager()->StopEffect(vfxHandle);
	}

	currentPlayFrame += playSpeed;
}

void CP_VisualEffect::PlayEndUpdate()
{
	if (pDxVfxManager->ExistEffect(vfxHandle)) return;	// �G�t�F�N�g�����݂��Ă���Ȃ�

	isPlaying = false;

	if (isLoop)
	{
		BeginPlayVFX();	// �Đ����Ȃ���
	}
	else
	{
		gameObject->SetDestroy();	// �j������
	}
}

void CP_VisualEffect::BeginPlayVFX()
{
	if (!pVFX)	// �G�t�F�N�g���ݒ肳��Ă��Ȃ��Ȃ�
	{
		HASHI_DEBUG_LOG("�G�t�F�N�g���Đ�����Ă��܂���");
		return;
	}

	// �Đ����Ȃ�
	if (isPlaying) return;
	// �g���~���O���Ȃ��Ȃ�
	if (!isTrimming) playStartFrame = 0;

	// �Đ����ăn���h�����擾
	Transform& transform = GetTransform();
	vfxHandle = pDxVfxManager->Play(pVFX->GetEffect(),
		transform.GetPosition(),
		transform.GetScale(),
		playStartFrame);

	currentPlayFrame = static_cast<float>(playStartFrame);

	// �Đ����x��ύX
	SetPlaySpeed(playSpeed);
	isPlaying = true;
}

void CP_VisualEffect::StopPlayVFX()
{
	isPlaying = false;
}

void CP_VisualEffect::ImGuiDebug()
{
	// ���[�v
	ImGui::Checkbox("IsLoop", &isLoop);

	// �Đ����x
	if (ImGui::DragFloat("Speed", &playSpeed, 0.001f, 0.0f, 100.0f))
	{
		SetPlaySpeed(playSpeed);
	}

	// �G�t�F�N�g���擾
	std::string vfxName = "Null";
	if (pVFX)
		vfxName = pVFX->GetAssetName();
	if (AssetGetter::ImGuiGetCombobox<VisualEffect>("vfx", vfxName))
	{
		pVFX = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}

	// �͈͎w��
	ImGui::Checkbox("IsTrimming", &isTrimming);
	if (isTrimming)
		ImGui::DragIntRange2("PlayRange", &playStartFrame, &playEndFrame, 1.0f, 0, 1000);
}
