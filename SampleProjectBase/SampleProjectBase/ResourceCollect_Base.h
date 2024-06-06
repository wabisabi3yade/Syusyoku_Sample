#pragma once

#include "Texture.h"
#include "Material.h"
#include "Model.h"

/// <summary>
/// ���\�[�X���Ǘ�������N���X
/// </summary>
/// <typeparam name="T">�Ǘ����郊�\�[���̌^</typeparam>
template <typename T>
class ResourceCollect_Base
{
protected:
	virtual ~ResourceCollect_Base();
	// �g���Ă��郂�f���̔z�񃊃X�g
	std::unordered_map<std::string, std::unique_ptr<T>> resourceList;

public:
	/// <summary>
	/// �����̖��O�̃��\�[�X�����邩�m�F����
	/// </summary>
	/// <param name="_checkName"></param>
	/// <returns></returns>
	bool GetIsImported(std::string _checkName);

	bool SetResource(std::unique_ptr<T> _pResource, std::string _setName);

	/// <summary>
	/// ���\�[�X��const�|�C���^���擾����
	/// </summary>
	/// <param name="_getName">�擾���閼�O</param>
	/// <returns>�w�肵��const�|�C���^</returns>
	const T* GetConstResource(std::string _getName);

	/// <summary>
	/// ���\�[�X�̐��|�C���^���擾����typename
	/// </summary>
	/// <param name="_getName">�擾���閼�O</param>
	/// <returns>�w�肵���|�C���^</returns>
	T* GetResource(std::string _getName);
};

template<typename T>
inline ResourceCollect_Base<T>::~ResourceCollect_Base()
{

}

template<typename T>
inline bool ResourceCollect_Base<T>::GetIsImported(std::string _checkName)
{
	// ���O��T������
	auto itr = resourceList.find(_checkName);
	// ���X�g���ɓ����Ă�Ȃ�
	if (itr != resourceList.end())
	{
		return true;
	}

	return false;
}

template<typename T>
inline bool ResourceCollect_Base<T>::SetResource(std::unique_ptr<T> _pResource, std::string _setName)
{
	// ���\�[�X�̖��O�����łɂ���Ȃ�
	if (GetIsImported(_setName))
	{
		std::string message = "���Ƀ��\�[�X������̂ŃZ�b�g�ł��܂���" + _setName;
		ImGuiDebugLog::AddDebugLog(message);
		return false;
	}
	// ���\�[�X��nullptr�Ȃ�
	else if (_pResource == nullptr)
	{
		std::string message = "���\�[�X��nullptr�ł�" + _setName;
		ImGuiDebugLog::AddDebugLog(message);
		return false;
	}

	// �z��Ɋm�ۂ���
	resourceList.emplace(_setName, std::move(_pResource));
	return true;
}

template<typename T>
inline const T* ResourceCollect_Base<T>::GetConstResource(std::string _getName)
{
	// ���f�������[�h����Ă��Ȃ��Ȃ�
	if (!GetIsImported(_getName))
	{
		std::string message = "���[�h����Ă��Ȃ����\�[�X�ł��@" + _getName;
		ImGuiDebugLog::AddDebugLog(message);
		return nullptr;
	}

	return resourceList[_getName].get();
}

template<typename T>
inline T* ResourceCollect_Base<T>::GetResource(std::string _getName)
{
	// ���f�������[�h����Ă��Ȃ��Ȃ�
	if (!GetIsImported(_getName))
	{
		std::string message = "���[�h����Ă��Ȃ����\�[�X�ł��@" + _getName;
		ImGuiDebugLog::AddDebugLog(message);
		return nullptr;
	}

	return resourceList[_getName].get();
}
