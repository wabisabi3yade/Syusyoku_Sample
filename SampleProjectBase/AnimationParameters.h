#pragma once
#include "AnimationParameterType.h"

/// @brief �A�j���[�V�����R���g���[���[���Ŏg�p����p�����[�^���X�g
class AnimationParameters : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{


	/// @brief �p�����[�^���X�g
	std::unordered_map<std::string, HashiTaku::AnimParam::conditionValType> animParameters;
public:
	AnimationParameters() {}
	~AnimationParameters() {}

	template<typename T> requires std::same_as<T, bool> || std::same_as<T, int> || std::same_as<T, float>
	void AddParameter(std::string _paramNames = "new Parameter");

	/// @brief �w�肵��bool�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _isBool �Z�b�g����l
	void SetBool(const std::string& _paramName, bool _isBool);

	/// @brief �w�肵��int�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _intVall �Z�b�g����l
	void SetInt(const std::string& _paramName, int _intVal);

	/// @brief �w�肵��float�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _floatVal �Z�b�g����l
	void SetFloat(const std::string& _paramName, float _floatVal);

	/// @brief �w�肵��bool�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _isBool �擾����l
	bool GetBool(const std::string& _paramName);

	/// @brief �w�肵��int�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _intVall �擾����l
	int GetInt(const std::string& _paramName);

	/// @brief �w�肵��float�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _floatVal �擾����l
	float GetFloat(const std::string& _paramName);

	/// @brief �p�����[�^���̃��X�g���擾����
	/// @param _nameList ������閼�O�z��
	void GetNameList(std::vector<const std::string*>& _nameList);

	/// @brief ���O����l�̃A�h���X��Ԃ�
	/// @param _name ���O
	/// @return �l�̃A�h���X(�Ȃ��Ȃ�nullptr)
	const HashiTaku::AnimParam::conditionValType* GetValueAddress(const std::string& _name) const;

	/// @brief ���O�ƈ�v�����p�����[�^���̃A�h���X��Ԃ�
	/// @param _name �T������̖��O
	/// @return �p�����[�^���̃A�h���X�̃A�h���X(�Ȃ��Ȃ�nullptr)
	const std::string* GetNameAddress(const std::string& _name) const;

	const std::string* GetBeginNameAddres() const;
	const HashiTaku::AnimParam::conditionValType* GetBeginValueAddres() const;

	// �p�����[�^�̌����擾
	u_int GetParameterCnt() const;

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

	/// @brief �s���Ȓl���Q�Ƃ��Ă��邩�`�F�b�N����(bool)
	/// @param _name �p�����[�^�[��
	/// @return ����Ȓl�ł��邩�H
	bool CheckBool(const std::string& _name);

	/// @brief �s���Ȓl���Q�Ƃ��Ă��邩�`�F�b�N����(int)
	/// @param _name �p�����[�^�[��
	/// @return ����Ȓl�ł��邩�H
	bool CheckInt(const std::string& _name);

	/// @brief �s���Ȓl���Q�Ƃ��Ă��邩�`�F�b�N����(float)
	/// @param _name �p�����[�^�[��
	/// @return ����Ȓl�ł��邩�H
	bool CheckFloat(const std::string& _name);

	/// @brief �^�C�v�𔻕ʂ���
	/// @param _parameter �p�����[�^�[
	/// @return ���ʃ^�C�v
	HashiTaku::AnimParam::TypeKind GetType(const HashiTaku::AnimParam::conditionValType& _parameter);

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

	HashiTaku::AnimParam::conditionValType addParam;

	if constexpr (std::is_same<T, bool>::value)
		addParam = false;

	else if constexpr(std::is_same<T, int>::value)
		addParam = 0;

	else if constexpr(std::is_same<T, float>::value)
		addParam = 0.0f;

	animParameters[_paramNames] = addParam;
}
