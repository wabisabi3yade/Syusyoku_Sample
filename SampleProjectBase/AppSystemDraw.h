#pragma once

class AssetDisplay;
class DX11BulletPhisics;

/// @brief ImGui�ŃV�X�e���֘A�̕`����s��
class AppSystemDraw : public HashiTaku::IImGuiUser
{
	/// @brief �σt���[�����[�g�N���X
	VariableFrameRate* pFrameRate;

	/// @brief �����蔻��
	DX11BulletPhisics* pBulletEngine;
	
public:
	AppSystemDraw(VariableFrameRate& _frameRate, DX11BulletPhisics& _bulletEngine);
	~AppSystemDraw() {}

private:
	void ImGuiSetting() override;
};

