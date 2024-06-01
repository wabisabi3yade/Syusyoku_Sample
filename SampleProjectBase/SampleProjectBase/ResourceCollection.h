#pragma once
#include "Singleton_Base.h"
class Resource_Base;

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
	template<class T> T* SetResource(std::string _resourceName, std::unique_ptr<T> _resourcePtr);

	/// <summary>
	/// ���O�ɑΉ��������\�[�X�|�C���^���擾����
	/// </summary>
	/// <typeparam name="T">���̃��\�[�X�̃|�C���^�N���X</typeparam>
	/// <param name="_resourceName">���\�[�X�̖��O</param>
	/// <returns>���\�[�X�̐��|�C���^</returns>
	template<class T> T* GetResource(std::string _resourceName);
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
class Resource : Resource_Base
{
	std::unique_ptr<T> ptr;	// �|�C���^�N���X
public:
	Resource(std::unique_ptr<T> _ptr) : ptr(_ptr) {};
	~Resource() {};

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
		MessageError(message.c_str());
		return nullptr;
	}

	// ���\�[�X�𐶐����A�z��ɓ����
	std::unique_ptr<Resource> resource = std::make_unique<Resource>(std::move(_resourcePtr));
	resources.emplace(_resourceName, resource);
	
	// ���\�[�X�|�C���^��Ԃ�
	return resource.GetPtr();
}

template<class T>
inline T* ResourceCollection::GetResource(std::string _resourceName)
{
	// ���O����T��
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())
	{
		std::string message = "���[�h����Ă��Ȃ��̂Ŏ擾�ł��܂���@" + _resourceName;
		MessageError(message.c_str());
		return nullptr;
	} 

	// ���|�C���^�����炤
	Resource_Base* basePtr = itr->second.get();
	// Resource�N���X�ɃL���X�g�ϊ����A���\�[�X�̃|�C���^�����炤
	Resource* resource = dynamic_cast<Resource*>(basePtr);
	T* retPtr = resource->GetPtr();

	return retPtr;	// ���\�[�X�̃|�C���^��Ԃ�
}
