#pragma once

namespace HashiTaku
{
	/// @brief ���x�����Ƃɐݒ肷��p�����[�^
	class CameraShakeParameter : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief �h�������x�N�g��
		DXSimp::Vector3 shakeVec;

		/// @brief �h��̗�
		float power{ 1.0f };

		/// @brief �h�炷����
		float time{ 0.3f };

		/// @brief �h�炷���x
		float speed{ 1.0f };

		/// @brief �t�F�[�h�A�E�g���邩�H
		bool isFadeOut{ false };

		CameraShakeParameter() = default;
		~CameraShakeParameter() = default;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}


