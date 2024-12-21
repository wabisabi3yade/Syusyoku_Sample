#pragma once
#include "CP_UIRenderer.h"
#include "IUISlider.h"

namespace HashiTaku
{
	class CP_UIRenderer;

	/// @brief UI�̃X���C�_�[�������s���N���X
	class CP_UISlider : public CP_UIRenderer, public IUISlider
	{
		/// @brief �X���C�_�[�摜�̒��S���W
		DirectX::SimpleMath::Vector3 sliderCenterPos;

		/// @brief �X���C�_�[�摜�̃X�P�[��
		DirectX::SimpleMath::Vector2 sliderScale;

		/// @brief ���݂̒l
		float currentValue;

		/// @brief �ő�l
		float maxValue;

		/// @brief �ŏ��l
		float minValue;
	public:
		CP_UISlider();
		~CP_UISlider() {}

		void Init() override;
		void OnChangeScale() override;
		void OnChangePosition() override;
		void OnChangeRotation() override;

		/// @brief ���݂̒l�������ŃZ�b�g(0.0�`1.0)
		/// @param _curRatio ���݂̊���
		void SetCurrentRatio(float _curRatio);

		/// @brief ���݂̒l���Z�b�g
		/// @param _curVal ���݂̒l
		void SetCurrentValue(float _curVal);

		/// @brief �ő�l���Z�b�g
		/// @param _maxVal �ő�l
		void SetMaxValue(float _maxVal);

		/// @brief �ŏ��l���Z�b�g
		/// @param _minVal �ŏ��l
		void SetMinValue(float _minVal);

		json Save() override;
		void Load(const json& _data) override;

	private:
		/// @brief �X���C�_�[�摜�ɓK�p����
		void ApplySlider();

		/// @brief �|���S���Đ���
		void ReCreatePolygon() override;

		void ImGuiDebug() override;
	};
}