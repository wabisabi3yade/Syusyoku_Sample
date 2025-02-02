#pragma once

namespace HashiTaku
{
	struct AcceptParryInfo
	{
		/// @brief �p���B�̋��x�ɂ��{��
		float parryStrengthRate{ 1.0f };
	};

	/// @brief �p���B���󂯗������U�����������̃C���^�[�t�F�[�X(�v���C���[���G�̍U�����p���B�@���@�G�̕��̃C���^�[�t�F�[�X)
	class IParryAccepter
	{

	public:
		IParryAccepter() = default;
		virtual ~IParryAccepter() = default;

		/// @brief �p���B���ꂽ�Ƃ��̏���
		/// @param _acceptInfo �p���B����̏��
		virtual void OnAcceptParry(const AcceptParryInfo& _acceptInfo) = 0;
	};
}


