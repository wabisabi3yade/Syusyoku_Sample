#pragma once

namespace HashiTaku
{
	/// @brief �g�����X�t�H�[����K�v�Ƃ���C���^�[�t�F�[�X
	class ITransformSetter
	{

	public:
		ITransformSetter() = default;
		~ITransformSetter() = default;

		/// @brief �g�����X�t�H�[���̃Z�b�^�[
		/// @param _transform �g�����X�t�H�[��
		virtual void SetTransform(const Transform& _transform) = 0;
	};
}