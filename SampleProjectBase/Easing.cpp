#include "pch.h"
#include "Easing.hpp"

#include "EaseFunction.hpp"

using namespace HashiTaku;

// static変数初期化
std::vector<std::unique_ptr<Ease_Base>> Easing::easeList;
std::unordered_map<std::string, EaseKind> Easing::easeDictionary;
std::unordered_map<std::string, EaseFuncKind> Easing::funcDictionary;

void HashiTaku::Easing::GetEase(EaseFunction& _easeFunc, EaseType _easeType)
{
	u_int easeId = static_cast<u_int>(_easeType.ease);

	// イージングの関数オブジェクトを参照にセットする
	switch (_easeType.funcType)
	{
	case HashiTaku::EaseFuncKind::In:
		_easeFunc = [=](float x) { return easeList[easeId]->EaseIn(x); };
		break;
	case HashiTaku::EaseFuncKind::Out:
		_easeFunc = [=](float x) { return easeList[easeId]->EaseOut(x); };
		break;
	case HashiTaku::EaseFuncKind::InOut:
		_easeFunc = [=](float x) { return easeList[easeId]->EaseInOut(x); };
		break;
	default:
		HASHI_DEBUG_LOG("不正なアクセスです");
		break;
	}
}

void HashiTaku::Easing::ImGuiSelect(EaseFunction& _easeFunc, EaseType& _nowEase)
{
	std::string curEaseName = EaseToString(_nowEase.ease);
	std::string curFuncName = FuncToString(_nowEase.funcType);
	
	bool isEaseChange = ImGuiMethod::ComboBox(TO_UTF8("イージング"), curEaseName, GetEaseNames());

	bool isFuncChange = ImGuiMethod::ComboBox(TO_UTF8("関数"), curFuncName, GetFuncNames());

	if (isEaseChange || isFuncChange)
	{
		_nowEase.ease = easeDictionary[curEaseName];
		_nowEase.funcType = funcDictionary[curFuncName];
		
		GetEase(_easeFunc, _nowEase);
	}
}

void HashiTaku::Easing::Init()
{
	// イージングクラスのオブジェクトをを作成
	for (u_int e_i = 0; e_i < static_cast<u_int>(EaseKind::MaxNum); e_i++)
		MakeEase(e_i);

	funcDictionary["In"] = EaseFuncKind::In;
	funcDictionary["Out"] = EaseFuncKind::Out;
	funcDictionary["InOut"] = EaseFuncKind::InOut;
}

void HashiTaku::Easing::MakeEase(u_int _easeId)
{
	std::string easeName;
	std::unique_ptr<Ease_Base> easeClass;

	EaseKind ease = static_cast<EaseKind>(_easeId);
	switch (ease)
	{
	case EaseKind::Linear:
		easeClass = std::make_unique<Ease_Linear>();
		easeName = "Linear"; break;

	case EaseKind::Sine:
		easeClass = std::make_unique<Ease_Sine>(); 
		easeName = "Sine"; break;

	case EaseKind::Quad: easeClass = std::make_unique<Ease_Quad>(); 
		easeName = "Quad"; break;

	case EaseKind::Cubic:  easeClass = std::make_unique<Ease_Cubic>(); 
		easeName = "Cubic"; break;

	case EaseKind::Quart:  easeClass = std::make_unique<Ease_Quart>(); 
		easeName = "Quart"; break;

	case EaseKind::Back:  easeClass = std::make_unique<Ease_Back>();
		easeName = "Back"; break;

	case EaseKind::Elastic:  easeClass = std::make_unique<Ease_Elastic>(); 
		easeName = "Elastic"; break;

	case EaseKind::Bounce:  easeClass = std::make_unique<Ease_Bounce>();
		easeName = "Bounce"; break;

	default:
		HASHI_DEBUG_LOG("イージング作成失敗");
		break;
	}

	easeList.push_back(std::move(easeClass));
	easeDictionary[easeName] = ease;
}

std::string HashiTaku::Easing::EaseToString(EaseKind _type)
{
	for (auto& pair : easeDictionary)
	{
		if (pair.second == _type)
			return pair.first;
	}

	assert(!"イージングの種類から探せませんでした");
	return "";
}

std::string HashiTaku::Easing::FuncToString(EaseFuncKind _type)
{
	for (auto& pair : funcDictionary)
	{
		if (pair.second == _type)
			return pair.first;
	}

	assert(!"関数の種類から探せませんでした");
	return "";
}

std::vector<std::string> HashiTaku::Easing::GetEaseNames()
{
	std::vector<std::string> s;
	for (auto& p : easeDictionary)
		s.push_back(p.first);

	return s;
}

std::vector<std::string> HashiTaku::Easing::GetFuncNames()
{
	std::vector<std::string> s;
	for (auto& p : funcDictionary)
		s.push_back(p.first);

	return s;
}
