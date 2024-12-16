#pragma once

namespace HashiTaku
{
	/// @brief UI�̃X���C�_�[�̃C���^�[�t�F�[�X
	class IUISlider
	{

	public:
		IUISlider() = default;
		virtual ~IUISlider() = default;


		/// @brief ���݂̒l�������ŃZ�b�g(0.0�`1.0)
		/// @param _curRatio ���݂̊���
		virtual void SetCurrentRatio(float _curRatio) = 0;

		/// @brief ���݂̒l���Z�b�g
		/// @param _curVal ���݂̒l
		virtual void SetCurrentValue(float _curVal) = 0;

		/// @brief �ő�l���Z�b�g
		/// @param _maxVal �ő�l
		virtual void SetMaxValue(float _maxVal) = 0;

		/// @brief �ŏ��l���Z�b�g
		/// @param _minVal �ŏ��l
		virtual void SetMinValue(float _minVal) = 0;
	};
}