#pragma once
#include "AnimationParameterType.h"
#include "IObserever.h"

// �O���錾
namespace HashiTaku
{
	namespace AnimParam
	{
		struct NotificationData;
	}
}

/// @brief �A�j���[�V�����R���g���[���[���Ŏg�p����p�����[�^���X�g
class AnimationParameters : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief �p�����[�^���X�g
	std::unordered_map<std::string, HashiTaku::AnimParam::conditionValType> animParameters;

	/// @brief �g���K�[�̏�Ԃ����Z�b�g���߂̔z��
	std::list<TriggerType*> pResetTriggers;

	/// @brief �p�����[�^�폜���̒ʒm�T�u�W�F�N�g
	std::unique_ptr<HashiTaku::Subject<HashiTaku::AnimParam::NotificationData>> pAnimParamSubject;
public:
	AnimationParameters();
	~AnimationParameters() {}

	/// @brief �p�����[�^��ǉ�����
	/// @tparam T �p�����[�^�̌^
	/// @param _paramNames �p�����[�^��
	template<HashiTaku::AnimParam::AnimParamConcept T>
	void AddParameter(std::string _paramNames = "new Parameter");

	/// @brief �p�����[�^�̖��O��t���ւ���
	/// @param _prevName �ύX�O�̖��O
	/// @param _changeName �ύX��̖��O
	void RenameParameter(const std::string& _prevName, std::string _changeName);

	/// @brief �p�����[�^���폜����
	/// @param _paramNames �p�����[�^��
	/// @return �폜�����}�b�v�z��̎��̃C�e���[�^
	void RemoveParameter(const std::string& _paramNames);

	/// @brief �g���K�[�̏�Ԃ����Z�b�g����
	void ResetTrigger();

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

	/// @brief �w�肵���g���K�[�ϐ���true�ɂ���
	/// @param _paramName �p�����[�^�[��
	void SetTrigger(const std::string& _paramName);

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

	/// @brief �w�肵���g���K�[�ϐ��̏�Ԃ��擾
	/// @param _paramName �p�����[�^�[��
	/// @param _floatVal ���K�[�ϐ��̏��
	bool GetTrigger(const std::string& _paramName);

	/// @brief �p�����[�^���̃��X�g���擾����
	/// @param _nameList ������閼�O�z��
	void GetNameList(std::vector<const std::string*>& _nameList) const;

	/// @brief �p�����[�^���̃��X�g���擾����
	/// @param _nameList ������閼�O�z��
	void GetFloatNameList(std::vector<const std::string*>& _nameList) const;

	/// @brief ���O����l�̃A�h���X��Ԃ�
	/// @param _name ���O
	/// @return �l�̃A�h���X(�Ȃ��Ȃ�nullptr)
	const HashiTaku::AnimParam::conditionValType* GetValueAddress(const std::string& _name) const;

	/// @brief ���O�ƈ�v�����p�����[�^���̃A�h���X��Ԃ�
	/// @param _name �T������̖��O
	/// @return �p�����[�^���̃A�h���X�̃A�h���X(�Ȃ��Ȃ�nullptr)
	const std::string* GetNameAddress(const std::string& _name) const;

	// �p�����[�^�̌����擾
	u_int GetParameterCnt() const;

	/// @brief �p�����[�^�폜�T�u�W�F�N�g���擾
	/// @return �T�u�W�F�N�g�̎Q��
	HashiTaku::Subject<HashiTaku::AnimParam::NotificationData>& GetParamRemoveSubject();

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

	/// @brief �s���Ȓl���Q�Ƃ��Ă��邩�`�F�b�N����(�g���K�[)
	/// @param _name �p�����[�^�[��
	/// @return ����Ȓl�ł��邩�H
	bool CheckTrigger(const std::string& _name);

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


template<HashiTaku::AnimParam::AnimParamConcept T>
inline void AnimationParameters::AddParameter(std::string _paramNames)
{
	// ���O�𕡂��Ȃ��悤�ɂ���
	NotDuplicateParamName(_paramNames);

	HashiTaku::AnimParam::conditionValType addParam;

	if constexpr (std::is_same<T, bool>::value)
		addParam = false;

	else if constexpr (std::is_same<T, int>::value)
		addParam = 0;

	else if constexpr (std::is_same<T, float>::value)
		addParam = 0.0f;

	else if constexpr (std::is_same<T, TriggerType>::value)
	{
		TriggerType t;
		addParam = t;
	}

	animParameters[_paramNames] = addParam;

	// �g���K�[�^�Ȃ烊�Z�b�g�z��ɒǉ�
	if (TriggerType* pTrigger = std::get_if<TriggerType>(&animParameters[_paramNames]))
		pResetTriggers.push_back(pTrigger);
}
