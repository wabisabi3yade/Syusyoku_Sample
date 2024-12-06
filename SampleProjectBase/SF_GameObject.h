#pragma once
#include "GameObject.h"

// �V�[�����֐�
namespace SceneFunction
{
	// �I�u�W�F�N�g
	class ObjectFunc
	{
		/// @brief �V�[���I�u�W�F�N�g�ɒǉ�����
		/// @param _pAddObject �ǉ�����I�u�W�F�N�g
		/// @return �ǉ������I�u�W�F�N�g��Ԃ�
		static GameObject& AddSceneObject(std::unique_ptr<GameObject> _pAddObject);

	public:
		/// @brief �I�u�W�F�N�g��V������������
		/// @param _obj �ΏۃI�u�W�F�N�g
		/// @param _worldPos ����������W
		/// @return 
		static GameObject& Instantiate(const GameObject& _obj, const DirectX::SimpleMath::Vector3& _worldPos);

		/// @brief ��I�u�W�F�N�g���쐬���A�V�[���I�u�W�F�N�g�ɒǉ�
		/// @param _objectName �I�u�W�F�N�g��
		/// @return ��I�u�W�F�N�g
		static GameObject& CreateEmpty(const std::string& _objectName);

		/// @brief �I�u�W�F�N�g���폜����
		/// @param _deleteName �폜���閼�O
		static void DeleteObject(const std::string& _deleteName);
	};

}

