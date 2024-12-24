#pragma once
#include "TransCondition_Base.h"

namespace HashiTaku
{
	/// @brief bool�^�ϐ������āA�J�ڏ����B�����Ă��邩
	class BoolTransCondition : public TransCondition_Base
	{
		/// @brief bool�^���������������߂̔���
		enum class JudgeType
		{
			True,
			False
		};

#ifdef EDIT
		/// @brief ����^�C�v�̕�����
		static std::vector<std::string> judgeTypeStrings;
#endif
	private:
		/// @brief ���ʃ^�C�v
		JudgeType judgeType;
	public:
		BoolTransCondition(const std::string& _parameterName);
		~BoolTransCondition() {}

		bool IsCondition(AnimParam::conditionValType _checkValue)const override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}