#pragma once

namespace HashiTaku
{
	/// @brief �[�x�������݂��s���������̃C���^�[�t�F�[�X
	class IApplyShadowDepth
	{
	public:
		IApplyShadowDepth() = default;
		virtual ~IApplyShadowDepth() = default;

		/// @brief �[�x�������݂��s��
		virtual void WriteDepth() = 0;
	};
}
