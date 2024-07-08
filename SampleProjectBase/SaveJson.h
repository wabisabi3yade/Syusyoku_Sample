#pragma once
#include "GameObject.h"


// json�t�@�C���Ƀp�����[�^���Z�[�u����
class SaveJson
{
	
public:
	/// <summary>
	/// �I�u�W�F�N�g���Z�[�u����
	/// </summary>
	/// <param name="_saveObject">�Z�[�u����I�u�W�F�N�g</param>
	/// <returns>�����t���O</returns>
	static bool SaveObject(GameObject& _saveObject);

	// �Q�[���I�u�W�F�N�g��Json�t�@�C������ǂݍ���
	static void LoadObject(GameObject& _gameObject, const nlohmann::json& _j);
};


namespace DirectX::SimpleMath
{
	// Vector�^�����ꂼ�ꃁ���o�ϐ��ŕۑ�����Ƃ��ɖ��O�̌��ɂ��閼�O
	constexpr std::string JsonAddX() { return "_x"; }
	constexpr std::string JsonAddY() { return "_y"; }
	constexpr std::string JsonAddZ() { return "_z"; }
	constexpr std::string JsonAddW() { return "_w"; }

	// Vector�^�ۑ�
	static void SaveJsonVector2(const std::string _s, DirectX::SimpleMath::Vector2& _v,
		nlohmann::json& _j);
	static void SaveJsonVector3(const std::string _s, DirectX::SimpleMath::Vector3& _v,
		nlohmann::json& _j);
	static void SaveJsonVector4(const std::string _s, DirectX::SimpleMath::Vector4& _v,
		nlohmann::json& _j);
	// Vector�^�Ǎ�
	static void LoadVector2(const std::string _s, DirectX::SimpleMath::Vector2& _v, const nlohmann::json& _j);
	static void LoadVector3(const std::string _s, DirectX::SimpleMath::Vector3& _v, const nlohmann::json& _j);
	static void LoadVector4(const std::string _s, DirectX::SimpleMath::Vector4& _v, const nlohmann::json& _j);

}