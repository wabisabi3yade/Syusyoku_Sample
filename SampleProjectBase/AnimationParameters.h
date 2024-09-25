#pragma once
#include "AnimationParameterType.h"

/// @brief �A�j���[�V�����R���g���[���[���Ŏg�p����p�����[�^���X�g
class AnimationParameters : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief �p�����[�^���X�g
	std::unordered_map<std::string, conditionValType> animParameters;
public:
	AnimationParameters() {}
	~AnimationParameters() {}

	template<typename T> requires std::same_as<T, bool> || std::same_as<T, int> || std::same_as<T, float>
	void AddParameter(std::string _paramNames = "new Parameter");

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �R���g���[���[���Ńp�����[�^�������Ȃ��悤�ɂ���
	/// @param _nodename �ύX�������p�����[�^��
	void NotDuplicateParamName(std::string& _paramName);

	void ImGuiSetting() override;
	// ImGui�Ńp�����[�^��V�K�ɒǉ�����
	void ImGuiAddParam();
	// ImGui�Ńp�����[�^��\��
	void ImGuiDisplay();
};


template<typename T> requires std::same_as<T, bool> || std::same_as<T, int> || std::same_as<T, float>
inline void AnimationParameters::AddParameter(std::string _paramNames)
{
	// ���O�𕡂��Ȃ��悤�ɂ���
	NotDuplicateParamName(_paramNames);

	conditionValType addParam;

	if constexpr (std::is_same<T, bool>::value)
		addParam = false;

	else if constexpr(std::is_same<T, int>::value)
		addParam = 0;

	else if constexpr(std::is_same<T, float>::value)
		addParam = 0.0f;

	animParameters[_paramNames] = addParam;
}
