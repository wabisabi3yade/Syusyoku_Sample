#pragma once
#include "TransCondition_Base.h"

namespace HashiTaku
{
	/// @brief float�^�ϐ������āA�J�ڏ����B�����Ă��邩
	class FloatTransCondition : public TransCondition_Base
	{
	public:
		/// @brief float�^���������������߂̔���
		enum class JudgeType
		{
			Greater,	// ����
			Less,	// ��艺
			Equal	// ������
		};

#ifdef EDIT
		/// @brief ����^�C�v�̕�����
		static std::vector<std::string> judgeTypeStrings;
#endif // EDIT
	private:
		/// @brief ��r����l
		float compareVal;

		/// @brief ���ʃ^�C�v
		JudgeType judgeType;
	public:
		FloatTransCondition(const std::string& _parameterName);
		~FloatTransCondition() {}

		bool IsCondition(AnimParam::conditionValType _checkValue)const override;

		json Save() override;
		void Load(const json& _data) override;
	private:

		void ImGuiDebug() override;
	};
}