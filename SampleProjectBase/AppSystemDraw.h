#pragma once

class AssetDisplay;
class DX11BulletPhisics;

/// @brief ImGui�ŃV�X�e���֘A�̕`����s��
class AppSystemDraw : public Singleton_Base<AppSystemDraw>, public HashiTaku::IImGuiUser
{
	friend class Singleton_Base<AppSystemDraw>;

	/// @brief �σt���[�����[�g�N���X
	VariableFrameRate* pFrameRate;
	
public:
	void Init(VariableFrameRate& _variabeFrameRate);

private:
	AppSystemDraw();
	~AppSystemDraw() {}
	void ImGuiDebug() override;
};

