#pragma once

namespace HashiTaku
{
	class AssetDisplay;
	class DX11BulletPhisics;

	/// @brief ImGui�ŃV�X�e���֘A�̕`����s��
	class AppSystemDraw : public Singleton_Base<AppSystemDraw>, public IImGuiUser
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

		void ImGuiSceneTimeScale();

		// �����G���W���֌W
		void ImGuiPhisics();

		// Vfx�֌W
		void ImGuiEffekseer();

		// �����_�[�^�[�Q�b�g�֌W
		void ImGuiRenderTarget();
	};
}

