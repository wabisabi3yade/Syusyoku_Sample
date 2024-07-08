#pragma once
#include "Singleton_Base.h"
class Resource_Base;

/* 
	�����K���@���̌��_���f���������(UE5�ɑ���)
	�X�P���^�����b�V���FSK
	�X�^�e�B�b�N���b�V���FSM
	�e�N�X�`���FT
	�}�e���A���FM
*/ 

// �Q�[���Ȃ��ł̗��p���郊�\�[�X(�e�N�X�`���A���f���Ȃ�)��unique_ptr�ŊǗ�����N���X
class ResourceCollection :
	public Singleton_Base<ResourceCollection>
{
	friend class Singleton_Base<ResourceCollection>;
	std::unordered_map<std::string, std::unique_ptr<Resource_Base>> resources;	// ���O���L�[�l�̃��\�[�X�z��
public:
	/// <summary>
	/// ���O�̃��\�[�X�����ɑ��݂��Ă邩�m�F����
	/// </summary>
	/// <param name="_resourceName">�m�F���郊�\�[�X��</param>
	/// <returns>���ɓ����Ă���t���O</returns>
	bool GetImpotred(std::string _resourceName);

	/// <summary>
	/// ���\�[�X���ƃ��\�[�X�̃|�C���^��z��ɒǉ�����
	/// </summary>
	/// <typeparam name="T">���̃��\�[�X�̃|�C���^�N���X</typeparam>
	/// <param name="_resourceName">�ǉ����閼�O</param>
	/// <param name="_resourcePtr">�ǉ�����|�C���^</param>
	/// <returns>�ǉ��������\�[�X�̐��|�C���^</returns>
	template<class T> T*  SetResource(std::string _resourceName, std::unique_ptr<T> _resourcePtr);

	/// <summary>
	/// ���O�ɑΉ��������\�[�X�|�C���^���擾����
	/// </summary>
	/// <typeparam name="T">���̃��\�[�X�̃|�C���^�N���X</typeparam>
	/// <param name="_resourceName">���\�[�X�̖��O</param>
	/// <returns>���\�[�X�̐��|�C���^</returns>
	template<class T> T* GetResource(std::string _resourceName);

	/// <summary>
	/// ���\�[�X�̃C���X�^���X���쐬���A�z��ɒǉ�����
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_resourceName"></param>
	/// <returns></returns>
	template<class T> T* CreateResource(std::string _resourceName);

	// ���g��\������
	void ImportDisplay();
};

// ���\�[�X���N���X
class Resource_Base
{
public:
	Resource_Base() {};
	virtual ~Resource_Base() {};
};

// ���\�[�X�N���X
template<class T>
class Resource : public Resource_Base
{
	std::unique_ptr<T> ptr;	// �|�C���^�N���X
public:
	// ���\�[�X�|�C���^�̏��L�������̃N���X�̃|�C���^�Ɉڍs������
	Resource(std::unique_ptr<T> _ptr) { ptr = std::move(_ptr); }
	~Resource() {}

	T* GetPtr() { return ptr.get(); }	// ���|�C���^�擾
};

template<class T>
inline T* ResourceCollection::SetResource(std::string _resourceName, std::unique_ptr<T> _resourcePtr)
{
	// �f�o�b�O���ɂ������O�Q�Ƃ����Ĕ�邩�m�F
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())	// ���ɖ��O����������
	{
		std::string message = "���Ƀ��\�[�X������܂�" + _resourceName;
		ImGuiDebugLog::Add(message);
		
		// �����郊�\�[�X�̃|�C���^��Ԃ�
		Resource<T>* resourceGetter = dynamic_cast<Resource<T>*>(itr->second.get());
		return  resourceGetter->GetPtr();
	}

	// �߂�l�̐��|�C���^���擾
	T* retResourcePtr = _resourcePtr.get();

	// ���\�[�X�𐶐����A�z��ɓ����
	std::unique_ptr<Resource_Base> setPtr = std::make_unique<Resource<T>>(std::move(_resourcePtr));
	resources.insert(std::pair<std::string, std::unique_ptr<Resource_Base>>(_resourceName, std::move(setPtr)));
	return retResourcePtr;
}

template<class T>
inline T* ResourceCollection::GetResource(std::string _resourceName)
{
	// ���O����T��
	auto itr = resources.find(_resourceName);
	if (itr == resources.end())
	{
		std::string message = "���[�h����Ă��Ȃ��̂Ŏ擾�ł��܂���@" + _resourceName;
		ImGuiDebugLog::Add(message);
		return nullptr;
	} 

	// ���|�C���^�����炤
	Resource_Base* basePtr = itr->second.get();
	// Resource�N���X�ɃL���X�g�ϊ����A���\�[�X�̃|�C���^�����炤
	Resource<T>* resource = dynamic_cast<Resource<T>*>(basePtr);
	T* retPtr = resource->GetPtr();

	return retPtr;	// ���\�[�X�̃|�C���^��Ԃ�
}

template<class T>
inline T* ResourceCollection::CreateResource(std::string _resourceName)
{
	// ���ɂ��̖��O�̃��\�[�X�������
	if (GetImpotred(_resourceName))
	{
		return nullptr;
	}
}
