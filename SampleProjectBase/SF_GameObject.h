#pragma once

class GameObject;

// �V�[�����֐�
namespace SceneFunction
{
	// �I�u�W�F�N�g
	namespace ObjectFunc
	{
		/// @brief �I�u�W�F�N�g��V������������
		/// @param _obj �ΏۃI�u�W�F�N�g
		/// @param _worldPos ����������W
		/// @return 
		GameObject& Instantiate(const GameObject& _obj, const DirectX::SimpleMath::Vector3& _worldPos);

		/// @brief 
		/// @param _objectName 
		/// @return 
		GameObject& CreateEmpty(std::string _objectName);
	}
	
}

