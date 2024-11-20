#include "pch.h"
#include "CP_VisualEffect.h"
#include "DX11EffecseerManager.h"
#include "AssetGetter.h"

CP_VisualEffect::CP_VisualEffect() : pVFX(nullptr), vfxHandle(-1), isPlaying(false)
{
}

nlohmann::json CP_VisualEffect::Save()
{
	auto data = Component::Save();

	if (pVFX)
		data["vfxName"] = pVFX->GetAssetName();

	return data;
}

void CP_VisualEffect::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

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

void CP_VisualEffect::BeginPlayVFX()
{
	if (!pVFX)	// �G�t�F�N�g���ݒ肳��Ă��Ȃ��Ȃ�
	{
		HASHI_DEBUG_LOG("�G�t�F�N�g���Đ�����Ă��܂���");
		return;
	}

	// �Đ����Ȃ�
	if (isPlaying) return;

	// �Đ�������
	DX11EffecseerManager::GetInstance()->
		Play(pVFX->GetEffect(), GetTransform().GetPosition());

	isPlaying = true;
}

void CP_VisualEffect::StopPlayVFX()
{
	isPlaying = true;
}

void CP_VisualEffect::ImGuiDebug()
{
	// �G�t�F�N�g���擾
	std::string vfxName = "Null";
	if (pVFX)
		vfxName = pVFX->GetAssetName();
	if (AssetGetter::ImGuiGetCombobox<VisualEffect>("vfx", vfxName))
	{
		pVFX = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}
}
