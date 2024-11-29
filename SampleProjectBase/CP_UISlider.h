#pragma once
#include "Component.h"
#include "IUISlider.h"

class CP_UIRenderer;

/// @brief UI�̃X���C�_�[�������s���N���X
class CP_UISlider : public Component, public IUISlider
{
	/// @brief UI�`��
	CP_UIRenderer* pUiRenderer;

	/// @brief 
	float startPos;

	/// @brief ���݂̒l
	float currentValue;

	/// @brief �ő�l
	float maxValue;

	/// @brief �ŏ��l
	float minValue;

	/// @brief �X���C�_�[�̑S�̂̒���
	float sliderAllLength;
public:
	CP_UISlider();
	~CP_UISlider() {}

	void Init() override;

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

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief �X���C�_�[�摜�ɓK�p����
	void ApplySlider();

	void ImGuiDebug() override;
};

