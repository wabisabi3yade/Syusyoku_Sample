#include "pch.h"
#include "CP_VisualEffect.h"
#include "DX11EffecseerManager.h"
#include "AssetGetter.h"
#include "GameObject.h"

CP_VisualEffect::CP_VisualEffect() : 
	pDxVfxManager(nullptr),
	pVFX(nullptr),
	vfxHandle(-1), 
	playSpeed(1.0f), 
	isPlaying(false),
	isLoop(false)
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

nlohmann::json CP_VisualEffect::Save()
{
	auto data = Component::Save();

	data["isLoop"] = isLoop;
	data["playSpeed"] = playSpeed;

	if (pVFX)
		data["vfxName"] = pVFX->GetAssetName();

	return data;
}

void CP_VisualEffect::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	HashiTaku::LoadJsonBoolean("isLoop", isLoop, _data);
	HashiTaku::LoadJsonFloat("playSpeed", playSpeed, _data);

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
	PlayEndUpdate();
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


	// �Đ����ăn���h�����擾
	Transform& transform = GetTransform();
	vfxHandle = pDxVfxManager->Play(pVFX->GetEffect(),
		transform.GetPosition(),
		transform.GetScale());

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
}
