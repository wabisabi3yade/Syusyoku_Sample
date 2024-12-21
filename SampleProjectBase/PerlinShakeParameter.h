#pragma once

namespace HashiTaku
{
	/// @brief �p�[�����m�C�Y�V�F�C�N����Ƃ��̃p�����[�^
	class PerlinShakeParameter : public IImGuiUser, public ISaveLoad
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

		PerlinShakeParameter() = default;
		~PerlinShakeParameter() = default;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}


