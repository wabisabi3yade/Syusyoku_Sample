#pragma once

namespace HashiTaku
{
	/// @brief �A�j���[�V�����J�[�u�ɂ��p�����[�^�ҏW(�G���~�[�g�Ȑ�)
	class AnimationCurve : public IImGuiUser, public ISaveLoad
	{
		// ��Ԃ̎��
		enum class InterpKind
		{
			Hermite, // �G���~�[�g
			Linear	// ���`
		};

		/// @brief �G���~�[�g�Ȑ��̓_�p�����[�^
		struct HermitePlotParam
		{
			/// @brief �l
			float value{ 0.0f };

			/// @brief ����
			float time{ 0.5f };

			/// @brief �x�N�g��
			float vector{ 0.5f };

			/// @brief ���܂ł̕�ԕ��@
			InterpKind toNextInterp{ InterpKind::Linear };

			bool operator==(const HermitePlotParam& _other)
			{
				return (this->value == _other.value) &&
					(this->time == _other.time) &&
					(this->vector == _other.vector);
			}
		};

		/// @brief �Ȑ��Ŏg�p���Ă��鐧��_
		std::list<HermitePlotParam> plotPoints;

		/// @brief �C�[�W���O���
		EaseKind easeKind;

		/// @brief �G���~�[�g�Ȑ����g�p���邩�H�i�g�p���Ȃ��ꍇ�A���`�j
		bool isUsePlot;
#ifdef EDIT
		// �J�[�u��
		std::string curveName{ "animationCurve" };

		// �ҏW�ΏۂɂȂ��Ă��邩�̃v���b�g�_
		HermitePlotParam* editingPlot{ nullptr };

		// �ҏW�ΏۂɂȂ��Ă��邩�̃v���b�g�_
		HermitePlotParam* dragingPlot{ nullptr };

		// ���x�x�N�g���ҏW�����H
		bool isVectorEdit{ false };

		// 1�t���[���O�̓h���b�O�����H
		bool prevIsDrag{ false };
#endif

	public:
		AnimationCurve();
		~AnimationCurve() {}

		// �J�[�u�����Z�b�g
		void SetCurveName(const std::string& _name);

		/// @brief �A�j���[�V�����J�[�u���擾����
		/// @param _time ����
		/// @return �l
		float GetValue(float _time)  const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief ���Ԃ���G���~�[�g�Ȑ����擾����
		/// @param _getTime 2�_�Ԃ̊���(0.0�`1.0)
		/// @param _p0 �O�̐���_
		/// @param _p1 ��̐���_
		/// @return ���Ԃ̒l
		float CalcHermiteCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const;

		/// @brief ���Ԃ��烊�j�A�Ȑ����擾����
		/// @param _getTime 2�_�Ԃ̊���(0.0�`1.0)
		/// @param _p0 �O�̐���_
		/// @param _p1 ��̐���_
		/// @return ���Ԃ̒l
		float CalcLinearCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const;


		/// @brief �v���b�g�_�̎��Ԃ������ɂ���
		/// @param _p0 �O
		/// @param _p1 ��
		/// @return ����ւ��邩�H
		static bool SortPointTime(const HermitePlotParam& _p0, const HermitePlotParam& _p1);

		/// @brief �e���̍ŏ��l�ƍő�l���擾����
		/// @return �e���̍ő�l�ƍŏ��l
		ImPlotRect GetPlotRect();

		/// @brief �v���b�g�_��ǉ�
		/// @param _time ����
		void AddPlot(float _time);

		/// @brief �v���b�g�_���폜
		/// @param _deletePlot �폜����v���b�g�_
		void RemovePlot(HermitePlotParam& _deletePlot);

		/// @brief �N���b�N�����Ƃ��낪�������邩�`�F�b�N
		/// @param _plot �v���b�g
		/// @param _clickX �N���b�N����X���W
		/// @param clickY Y���W
		/// @return ��������
		bool CheckReactPlot(const HermitePlotParam& _plot);

		/// @brief �N���b�N�����Ƃ��낪�������邩�`�F�b�N
		/// @param _posX�v���b�gX���W
		/// @param _posY �v���b�gY���W
		/// @param _clickX �N���b�N����X���W
		/// @param clickY Y���W
		/// @return ��������
		bool CheckReactPlot(float _posX, float _posY);

		/// @brief �C�[�W���O��ނ��Z�b�g
		/// @param _easeKind ���
		void SetEase(EaseKind _easeKind);

		/// @brief �n�_�ƏI�_�̃v���b�g���m�F
		/// @param _checkPlot �m�F����v���b�g
		/// @return �n�_�܂��͏I�_�̃v���b�g���H
		bool IsStartOrEndPlot(const HermitePlotParam* _checkPlot);

		void ImGuiDebug() override;

		// ImGui�Ńv���b�g�_��ҏW
		void ImGuiPlotEditing();

		// ImGui�Ńx�N�g���_��ҏW
		void ImEditVectorPoint();

		// ImGui�ŃO���t��`��
		void ImDrawGraph();

		// ImGui�Ńv���b�g�_��ǉ��E�폜���s��
		void ImAddPopPlot();
	};
}