#pragma once
#include "SaveJson.h"

class SaveJsonValue;

// Json�ɃZ�[�u����
namespace HashiTaku_JsonSave
{
	using namespace DirectX::SimpleMath;	// ���͈͓̔������g�p����
	using json =  nlohmann::json;	// ���͈͓̔������g�p����
	// �R���Z�v�g�Ŕz��ɒǉ��ł���^���𐧌�����
	template<typename T>
	concept AllowType =
		std::is_same_v<T, bool> || std::is_same_v<T, short> ||
		std::is_same_v<T, u_int> || std::is_same_v<T, int> ||
		std::is_same_v<T, float> || std::is_same_v<T, double> ||
		std::is_same_v<T, std::string> || std::is_same_v<T, Vector2> ||
		std::is_same_v<T, Vector3> || std::is_same_v<T, Vector4> ||
		std::is_same_v<T, Color>;

	// �e���v���[�g���g�p���Ă��Ȃ������k�N���X
	class JsonValue_Base
	{
		friend class SaveJsonValue;	//  SaveJsonValue�ł�����`�����Ȃ��悤�ɂ���

	public:
		JsonValue_Base() {};
		virtual void JasonSave(json& _j){};
	};

	// �z��Ƀe���v���[�g���g�p����ׂ̖{�̃N���X
	template <HashiTaku_JsonSave::AllowType T>
	class JsonValue : private JsonValue_Base
	{
		friend class SaveJsonValue;	//  SaveJsonValue�ł�����`�����Ȃ��悤�ɂ���
		std::string& name;	// �ϐ���
		T& value;	// �ϐ��̒l
	public:
		JsonValue() {};
		void JasonSave(json& _j)override;	// ���̒l��json�ɃZ�[�u����
	};

	template<HashiTaku_JsonSave::AllowType T>
	inline void JsonValue<T>::JasonSave(json& _j)
	{
		// �^���ɉ��������@��json�ɏ�������
		auto ti = typeid(T);
		if (ti == typeid(Vector3))	// ���Vector3�^���g����̂ŏ��
		{
			SaveVector3(name, value, _j);
		}
		else if (ti == typeid(Vector4) || ti == typeid(Color))	// Color��������
		{
			SaveVector4(name, value, _j);
		}
		else if (ti == typeid(Vector2))
		{
			SaveVector2(name, value, _j);
		}
		else
		{
			_j[name] = value;	// ���ڂ����
		}
	}

}

// Json�ɃZ�[�u���邽�߂̔z������N���X
class SaveJsonValue
{
	std::list<HashiTaku_JsonSave::JsonValue_Base> saveValues;	// �Z�[�u����ϐ��z��

public:
	SaveJsonValue() {};
	~SaveJsonValue() { saveValues.clear(); }

	// �z��ɒǉ�����
	template<HashiTaku_JsonSave::AllowType T> void Add(const std::string& _name, const T& _saveValue);

	// �����Ă�z���Json�t�@�C���ɏ�������
	void WriteJson(nlohmann::json& _jsonData);
};

template<HashiTaku_JsonSave::AllowType T>
inline void SaveJsonValue::Add(const std::string& _name, const T& _saveValue)
{
	HashiTaku_JsonSave::JsonValue<T> jsonVal;
	jsonVal.name = _name;	// �ϐ�������
	jsonVal.value = _saveValue;	// �l����
	
	saveValues.push_back(jsonVal);	// �ǉ�
}


