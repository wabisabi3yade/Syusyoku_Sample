#pragma once
#include "AnimationNode_Base.h"

class AnimationParameters;

/// @brief 2�ȏ�̃A�j���[�V�������u�����h����Ă���m�[�h
class BlendAnimationNode : public AnimationNode_Base
{
public:
	/// @brief �u�����h�X�y�[�X���Ŏg�p����A�j���[�V�����f�[�^(�u�����h�|�C���g)
	struct AnimPointData
	{
		/// @brief �A�j���[�V�����f�[�^
		const AnimationData* pAnimation{ nullptr };
		/// @brief �u�����h�l
		DirectX::SimpleMath::Vector2 blendValue;
	};

	/// @brief �p�������߂�Ƃ��Ƀu�����h����Ƃ���1�u�����h�p�����[�^
	struct BlendingData
	{
		/// @brief �A�j���[�V�����f�[�^
		const AnimationData* pAnimation{ nullptr };

		/// @brief �ǂꂾ���u�����h���邩�d��(���v��1.0)
		float blendWeight{ 0.0f };
	};

private:
	/// @brief �����Ƃ̃p�����[�^
	struct AxisParameter
	{
		/// @brief �u�����h�l�̖ڕW�ɂ���p�����[�^��
		std::string targetParamName;

		/// @brief �ő�͈͂̒l
		float maxRangeValue{ 1.0f };

		/// @brief �ŏ��͈͂̒l
		float minRangeValue{ 0.0f };

		/// @brief �u�����h�l�̈ړ�����
		float blendMoveTime{ 1.0f };

		/// @brief �^�[�Q�b�g�̊����ֈړ��C�[�W���O
		HashiTaku::EaseKind blendMoveEase{ HashiTaku::EaseKind::OutCubic };
	};

	/// @brief �|�C���g�Ƃ̋��������߂�\��
	struct PointToDistance
	{
		const BlendAnimationNode::AnimPointData* point{ nullptr };
		float distance = 0.0f;
	};

	/// @brief �u�����h�őΉ����Ă���ő厲��
	static constexpr unsigned short MAX_AXIS_CNT = 2;

	/// @brief �u�����h�őΉ����Ă���ŒᎲ��
	static constexpr unsigned short MIN_AXIS_CNT = 1;

	/// @brief  �u�����h����A�j���[�V�����f�[�^
	std::list<AnimPointData> animationPoints;

	/// @brief �����Ƃ̃p�����[�^�ϐ�
	std::vector<AxisParameter> axisParameters;

	/// @brief �A�j���[�V�����p�����[�^�̃|�C���^
	const AnimationParameters* pAnimParameters;

	/// @brief �A�j���[�V�����ԂŃu�����h����(���Ȃ��ꍇ�A�E�F�C�g���傫���A�j���[�V��������)
	bool isUseBlend;
public:
	BlendAnimationNode(const AnimationParameters& _animParams, std::string _nodeName);
	~BlendAnimationNode() {}

	/// @brief �A�j���[�V������ǉ�����
	/// @param _animData �A�j���[�V�����f�[�^
	void SetAnimationData(const std::string& _animName) override;

	/// @brief �u�����h�l�Ƃ���A�j���[�V�����p�����[�^�����擾
	/// @param _axisIdx ���ԍ�(0��X�A1��Y)
	/// @return �A�j���[�V�����p�����[�^��
	const std::string& GetTargetParamName(u_int _axisIdx) const;

	/// @brief �u�����h�l���ړ����鎞�Ԃ��擾
	/// @param _axisIdx ���ԍ�(0��X�A1��Y)
	/// @return �ړ�����
	float GetBlendMoveTime(u_int _axisIdx) const;

	/// @brief �u�����h�l���ړ�����C�[�W���O���擾
	/// @param _axisIdx ���ԍ�(0��X�A1��Y)
	/// @return �ړ��C�[�W���O
	HashiTaku::EaseKind GetBlendMoveEasing(u_int _axisIdx) const;

	void CalcRootMotionSpeed(const std::vector<BlendingData>& _blendData, DirectX::SimpleMath::Vector3& _outPos) const;

	/// @brief ���̐����擾�i�ő�2�j
	/// @return ���̐�
	u_int GetAxisCnt() const;

	/// @brief ���[�g���[�V�����̍��W���擾
	/// @param _ratio ����
	/// @param _blendDatas �u�����h�f�[�^ 
	/// @param _outPos ����
	void GetRootMotionPos(float _ratio, const std::vector<BlendingData>& _blendDatas, DirectX::SimpleMath::Vector3& _outPos) const;

	/// @brief �u�����h���邩�ǂ����擾
	/// @return �u�����h���邩�H
	bool GetIsUseBlend() const;

	/// @brief X���݂̂̂Ƃ��̃u�����h�y�A�T��
	/// @param _blendValue �u�����h�l
	/// @param _outData �u�����h�f�[�^���X�g
	void FindBlendingDataSingleAxis(float _blendValue, std::vector<BlendingData>& _outData) const;

	/// @brief Y���݂̂̂Ƃ��̃u�����h�y�A�T��
	/// @param _blendValues �u�����h�l
	/// @param _outData �u�����h�f�[�^���X�g
	void FindBlendPairTwoAxis(DirectX::SimpleMath::Vector2 _blendValues, std::vector<BlendingData>& _outData) const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �u�����h�l�ƃ|�C���g�̃u�����h�l�Ƃ̋���������������ׂ�
	/// @param _a A
	/// @param _b B
	/// @return ���ѕς���
	static bool CompareBlendDistance(const PointToDistance& _a, const PointToDistance& _b);

	/// @brief �u�����h�l��������������ׂ�
	/// @param _a A
	/// @param _b B
	/// @return ���ѕς���
	static bool CompareBlendvalue(const AnimPointData& _a, const AnimPointData& _b);

	/// @brief �u�����h�l�Ń\�[�g����
	void SortBlendPoint();

	/// @brief ���𑝂₷
	void AddAxisParam();

	/// @brief �������炷
	void RemoveAxisParam();

	/// @brief �A�j���[�V�����|�C���g�̐��擾
	/// @return �A�j���[�V�����|�C���g��
	u_int GetAnimPointCnt() const;

	void ImGuiDebug() override;

	/// @brief �u�����h���̃A�j���[�V����
	void ImGuiAnimationInBlend();

	// �����Ƃ̎��p�����[�^�𒲐�
	void ImGuiAxisParam();

	// �u�����h�̃^�[�Q�b�g�ϐ���ύX����
	void ImGuiChangeTargetParam();

	// �u�����h�f�[�^���Z�[�u����
	nlohmann::json SaveAxisParameter(const AxisParameter& _axisParam);

	// ���p�����[�^���Z�[�u����
	nlohmann::json SaveBlendPoint(const AnimPointData& _blendPoint);

	// ���p�����[�^�����[�h����
	void LoadAxisParameter(const nlohmann::json& _blendData);

	// �u�����h�f�[�^�����[�h����
	void LoadBlendPoint(const nlohmann::json& _blendData);
};

