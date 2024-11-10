#include "pch.h"
#include "AnimationParameters.h"

#include "AnimParamRemoveObserver.h"

using namespace HashiTaku::AnimParam;

constexpr float PARAM_EDIT_SPEED(0.001f);	// �ҏW�ňړ�����p�����[�^���x

AnimationParameters::AnimationParameters()
{
	pAnimParamSubject = std::make_unique<HashiTaku::Subject<NotificationData>>();
}

AnimationParameters::AnimationParameters(const AnimationParameters& _other)
{
	Copy(_other);
}

AnimationParameters& AnimationParameters::operator=(const AnimationParameters& _other)
{
	Copy(_other);

	return *this;
}

void AnimationParameters::RenameParameter(const std::string& _prevName, std::string _changeName)
{
	// ���O����C�e���[�^���擾
	auto deleteItr = animParameters.find(_prevName);
	if (deleteItr == animParameters.end()) return;

	// ��U�ۑ�
	auto tmpValue = std::move(deleteItr->second);
	
	// �V�����ǉ�
	NotDuplicateParamName(_changeName);
	animParameters[_changeName] = tmpValue;

	// �ʒm����
	NotificationData data;
	data.eventType = AnimParamObseveEvent::Rename;
	data.pDeleteName = &_prevName;
	data.pAfterName = &_changeName;
	pAnimParamSubject->NotifyAll(data);

	// �폜����
	animParameters.erase(deleteItr);
}

void AnimationParameters::RemoveParameter(const std::string& _paramNames)
{
	if (!animParameters.contains(_paramNames)) return;

	// �I�u�U�[�o�[�֍폜����p�����[�^����ʒm
	NotificationData data;
	data.eventType = AnimParamObseveEvent::Remove;
	data.pDeleteName = &_paramNames;
	pAnimParamSubject->NotifyAll(data);

	// �g���K�[�^�Ȃ烊�Z�b�g�z�񂩂�폜
	if (TriggerType* pTrigger = std::get_if<TriggerType>(&animParameters[_paramNames]))
		resetTriggers.remove(pTrigger);

	animParameters.erase(_paramNames);
}

void AnimationParameters::ResetTrigger()
{
	for (auto& pTrigger : resetTriggers)
		pTrigger->ResetTrigger();
}

bool AnimationParameters::IsContain(const std::string& _parameterName) const
{
	return animParameters.contains(_parameterName);
}

void AnimationParameters::SetBool(const std::string& _paramName, bool _isBool)
{
#ifdef EDIT
	if (!CheckBool(_paramName)) return;
#endif // EDIT

	animParameters[_paramName] = _isBool;
}

void AnimationParameters::SetInt(const std::string& _paramName, int _intVal)
{
#ifdef EDIT
	if (!CheckInt(_paramName)) return;
#endif // EDIT

	animParameters[_paramName] = _intVal;
}

void AnimationParameters::SetFloat(const std::string& _paramName, float _floatVal)
{
#ifdef EDIT
	if (!CheckFloat(_paramName)) return;
#endif // EDIT

	animParameters[_paramName] = _floatVal;
}

void AnimationParameters::SetTrigger(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckTrigger(_paramName)) return;
#endif // EDIT

	// �g���K�[��true�ɂ���
	TriggerType* t = std::get_if<TriggerType>(&animParameters[_paramName]);
	t->SetIsTrigger();
}

bool AnimationParameters::GetBool(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckBool(_paramName)) return false;
#endif // EDIT

	return std::get<bool>(animParameters[_paramName]);
}

int AnimationParameters::GetInt(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckInt(_paramName)) return 0;
#endif // EDIT

	return std::get<int>(animParameters[_paramName]);
}

float AnimationParameters::GetFloat(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckFloat(_paramName)) return 0.0f;
#endif // EDIT

	return std::get<float>(animParameters[_paramName]);
}

bool AnimationParameters::GetTrigger(const std::string& _paramName)
{
#ifdef EDIT
	if (!CheckTrigger(_paramName)) return false;
#endif // EDIT

	// ��Ԃ��擾
	TriggerType* t = std::get_if<TriggerType>(&animParameters[_paramName]);
	return t->GetIsTrigger();
}

void AnimationParameters::GetNameList(std::vector<const std::string*>& _nameList) const
{
	for (auto& param : animParameters)
	{
		_nameList.push_back(&param.first);
	}
}

void AnimationParameters::GetFloatNameList(std::vector<const std::string*>& _nameList) const
{
	for (auto& param : animParameters)
	{
		// float�^�Ȃ�
		if (std::holds_alternative<float>(param.second))
			_nameList.push_back(&param.first);
	}
}

const conditionValType* AnimationParameters::GetValueAddress(const std::string& _name) const
{
	if (!animParameters.contains(_name)) return nullptr;

	auto itr = animParameters.find(_name);
	return &itr->second;
}

const std::string* AnimationParameters::GetNameAddress(const std::string& _name) const
{
	if (!animParameters.contains(_name)) return nullptr;

	auto itr = animParameters.find(_name);
	return &itr->first;
}

HashiTaku::AnimParam::TypeKind AnimationParameters::GetParameterType(const std::string& _parameterName) const
{
	// �p�����[�^���X�g�ɂȂ��Ȃ�
	if (!animParameters.contains(_parameterName))
	{
		HASHI_DEBUG_LOG(_parameterName + "���Ȃ��̂Ō^���擾�ł��܂���ł���");
		return TypeKind::None;
	}
	
	const conditionValType& parameter = animParameters.at(_parameterName);

	// ���ʂ���
	TypeKind retType = TypeKind::Bool;
	if (std::holds_alternative<int>(parameter))
		retType = TypeKind::Int;
	else if (std::holds_alternative<float>(parameter))
		retType = TypeKind::Float;
	else if (std::holds_alternative<TriggerType>(parameter))
		retType = TypeKind::Trigger;

	return retType;
}

u_int AnimationParameters::GetParameterCnt() const
{
	return static_cast<u_int>(animParameters.size());
}

HashiTaku::Subject<HashiTaku::AnimParam::NotificationData>& AnimationParameters::GetParamRemoveSubject()
{
	return *pAnimParamSubject;
}

nlohmann::json AnimationParameters::Save()
{
	nlohmann::json parameterListData;

	for (auto& param : animParameters)
	{
		//���O
		nlohmann::json parameterData;
		parameterData["name"] = param.first;

		//�^
		TypeKind type = GetType(param.second);
		parameterData["type"] = type;

		//�l
		const std::string element = "value";
		switch (type)
		{
		case TypeKind::Bool:
			parameterData[element] = std::get<bool>(param.second);
			break;
		case TypeKind::Int:
			parameterData[element] = std::get<int>(param.second);
			break;
		case TypeKind::Float:
			parameterData[element] = std::get<float>(param.second);
			break;
		case TypeKind::Trigger:
			break;
		default:
			break;
		}

		parameterListData.push_back(parameterData);
	}

	return parameterListData;
}

void AnimationParameters::Load(const nlohmann::json& _data)
{
	for (auto& paramData : _data)
	{
		// �p�����[�^��
		std::string name;
		if (!HashiTaku::LoadJsonString("name", name, paramData)) continue;

		// �p�����[�^�^
		TypeKind type;
		if (!HashiTaku::LoadJsonEnum<TypeKind>("type", type, paramData)) continue;

		// �l
		conditionValType value;
		const std::string element = "value";
		switch (type)
		{
		case TypeKind::Bool:
		{
			bool isBool = false; HashiTaku::LoadJsonBoolean(element, isBool, paramData);
			value = isBool;
		}
		break;

		case TypeKind::Int:
		{
			int intVal = 0;  HashiTaku::LoadJsonInteger(element, intVal, paramData);
			value = intVal;
		}
		break;

		case TypeKind::Float:
		{
			float floatVal = 0.0f;  HashiTaku::LoadJsonFloat(element, floatVal, paramData);
			value = floatVal;
		}
		break;

		case TypeKind::Trigger:
		{
			TriggerType t;
			value = t;
		}

		break;
		default:
			break;
		}

		// �ǉ�
		animParameters[name] = value;
	}
}

void AnimationParameters::NotDuplicateParamName(std::string& _paramName)
{
	u_int loopCnt = 1;
	std::string startName = _paramName;

	// �������O��������胋�[�v
	while (animParameters.contains(_paramName))	
	{
		_paramName = startName + std::to_string(loopCnt);
		loopCnt++;
	}
}

bool AnimationParameters::CheckBool(const std::string& _name)
{
	// �s���Ȓl���Q�Ƃ��Ă��Ȃ����`�F�b�N
	if (!animParameters.contains(_name))
	{
		HASHI_DEBUG_LOG(_name + "��������܂���ł���");
		return false;
	}
	conditionValType getVal = animParameters[_name];
	if (!std::holds_alternative<bool>(getVal))
	{
		HASHI_DEBUG_LOG(_name + "��bool�^�ł͂���܂���");
		return false;
	}

	return true;
}

bool AnimationParameters::CheckInt(const std::string& _name)
{
	// �s���Ȓl���Q�Ƃ��Ă��Ȃ����`�F�b�N
	if (!animParameters.contains(_name))
	{
		HASHI_DEBUG_LOG(_name + "��������܂���ł���");
		return false;
	}
	conditionValType getVal = animParameters[_name];
	if (!std::holds_alternative<int>(getVal))
	{
		HASHI_DEBUG_LOG(_name + "��int�^�ł͂���܂���");
		return false;
	}

	return true;
}

bool AnimationParameters::CheckFloat(const std::string& _name)
{
	// �s���Ȓl���Q�Ƃ��Ă��Ȃ����`�F�b�N
	if (!animParameters.contains(_name))
	{
		HASHI_DEBUG_LOG(_name + "��������܂���ł���");
		return false;
	}
	conditionValType getVal = animParameters[_name];
	if (!std::holds_alternative<float>(getVal))
	{
		HASHI_DEBUG_LOG(_name + "��float�^�ł͂���܂���");
		return false;
	}
	return true;
}

bool AnimationParameters::CheckTrigger(const std::string& _name)
{
	// �s���Ȓl���Q�Ƃ��Ă��Ȃ����`�F�b�N
	if (!animParameters.contains(_name))
	{
		HASHI_DEBUG_LOG(_name + "��������܂���ł���");
		return false;
	}
	conditionValType getVal = animParameters[_name];
	if (!std::holds_alternative<TriggerType>(getVal))
	{
		HASHI_DEBUG_LOG(_name + "�̓g���K�[�^�ł͂���܂���");
		return false;
	}
	return true;
}

TypeKind AnimationParameters::GetType(const conditionValType& _parameter)
{
	// �ϐ��̌^���擾
	TypeKind retType = TypeKind::Bool;

	if (std::holds_alternative<int>(_parameter))
		retType = TypeKind::Int;
	else if (std::holds_alternative<float>(_parameter))
		retType = TypeKind::Float;
	else if (std::holds_alternative<TriggerType>(_parameter))
		retType = TypeKind::Trigger;

	return retType;
}

void AnimationParameters::Copy(const AnimationParameters& _other)
{
	if (this == &_other) return;

	// �p�����[�^�R�s�[
	animParameters = _other.animParameters;
	for (auto& param : animParameters)
	{
		// �g���K�[�̓��X�g��
		if (TriggerType* pTrigger = std::get_if<TriggerType>(&param.second))
			resetTriggers.push_back(pTrigger);
	}
}

void AnimationParameters::ImGuiDebug()
{
	ImGui::Text("Parameter");
	ImGuiAddParam();
	ImGuiDisplay();
}

void AnimationParameters::ImGuiAddParam()
{
#ifdef EDIT
	std::vector<std::string> str =
	{
		"Bool",
		"Int",
		"Float",
		"Trigger"
	};
	static u_int selectId = 0;

	ImGuiMethod::ComboBox("##ParamCombo", selectId, str);
	ImGui::SameLine();
	if (ImGui::Button("+"))
	{
		TypeKind paramType = static_cast<TypeKind>(selectId);

		switch (paramType)
		{
		case TypeKind::Bool:
			AddParameter<bool>();
			break;
		case TypeKind::Int:
			AddParameter<int>();
			break;
		case TypeKind::Float:
			AddParameter<float>();
			break;
		case TypeKind::Trigger:
			AddParameter<TriggerType>();
			break;

		default:
			assert(!"�s���ȃp�����[�^�^�C�v");
			break;
		}
	}
#endif // EDIT
}

void AnimationParameters::ImGuiDisplay()
{
#ifdef EDIT
	u_int id = 0;
	decltype(animParameters)::iterator itr = animParameters.begin();
	while (itr != animParameters.end())
	{
		ImGuiMethod::LineSpaceSmall();

		bool isDelete = false;
		std::string name = itr->first;
		char buf[IM_INPUT_BUF];	// �ϐ�����ύX
		strncpy_s(buf, name.c_str(), sizeof(buf));

		ImGui::PushID(itr->first.c_str());
		if (ImGui::InputText("##param", buf, IM_INPUT_BUF, ImGuiInputTextFlags_EnterReturnsTrue))	// ���̓t�B�[���h
		{
			// ���̖��O���폜���āA�V�����ǉ�����
			isDelete = true;
			itr = std::next(itr);
			RenameParameter(name, buf);
			ImGui::PopID();
			continue;
		}

		ImGui::SameLine();
		// �p�����[�^����
		if (bool* pBool = std::get_if<bool>(&itr->second))	// bool
			ImGui::Checkbox("##bool", pBool);

		else if (int* pInt = std::get_if<int>(&itr->second))// int
		{
			ImGuiMethod::PushItemSmallWidth();
			ImGui::DragInt("##int", pInt);
			ImGui::PopItemWidth();
		}
		else if (float* pFloat = std::get_if<float>(&itr->second))	// float
		{
			ImGuiMethod::PushItemSmallWidth();
			ImGui::DragFloat("##float", pFloat, PARAM_EDIT_SPEED);
			ImGui::PopItemWidth();
		}
		ImGui::SameLine();
		if (ImGui::Button("X"))	// �폜
			isDelete = true;

		if (isDelete)
		{
			std::string deleteKey = itr->first;
			auto deleteItr = animParameters.find(deleteKey);
			itr = std::next(deleteItr);

			RemoveParameter(deleteKey);
		}
		else
			itr++;

		ImGui::PopID();
	}

#endif // EDIT
}
