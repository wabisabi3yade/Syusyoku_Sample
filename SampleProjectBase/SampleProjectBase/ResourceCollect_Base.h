#pragma once

/// <summary>
/// ���\�[�X���Ǘ�������N���X
/// </summary>
/// <typeparam name="T">�Ǘ����郊�\�[���̌^</typeparam>
template <typename T>
class ResourceCollect_Base
{
protected:
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
	/// ���\�[�X
	/// </summary>
	/// <param name="_getName"></param>
	/// <returns></returns>
	const T* GetResource(std::string _getName);
};

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
	if (GetIsImported())
	{
		std::string message = "���Ƀ��\�[�X������̂ŃZ�b�g�ł��܂���" + _setName;
		MessageError(message.c_str());
		return false;
	}
	// ���\�[�X��nullptr�Ȃ�
	else if (_pResource == nullptr)
	{
		std::string message = "���\�[�X��nullptr�ł�" + _setName;
		MessageError(message.c_str());
		return false;
	}

	// �z��Ɋm�ۂ���
	resourceList.emplace(_setName, std::move(_pResource));
	return true;
}

template<typename T>
inline const T* ResourceCollect_Base<T>::GetResource(std::string _getName)
{
	// ���f�������[�h����Ă��Ȃ��Ȃ�
	if (!GetIsImported(_getName))
	{
		std::string message = "���[�h����Ă��Ȃ����\�[�X�ł��@" + _getName;
		MessageError(message.c_str());
		return {};
	}

	return resourceList[_getName].get();
}
