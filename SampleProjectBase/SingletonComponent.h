#pragma once
#include "Component.h"

class ComponentRespawner_Base;

/// @brief �V���O���g���ȃR���|�[�l���g
template<class T>
class SingletonComponent : public Component
{
	// �R���|�[�l���g���̂̐����̓R���|�[�l���g�����N���X�ɔC����
	friend class ComponentRespawner_Base;

	/// @brief �C���X�^���X
	static T* pInstance;

public:
	/// @brief �C���X�^���X���擾
	/// @return �C���X�^���X
	static inline T* GetInstance();

	/// @brief �폜����
	void OnDestroy() override;
protected:
	SingletonComponent() {}
	virtual ~SingletonComponent() {}

	void Init() override;
private:
	SingletonComponent(const  SingletonComponent&) = delete;
	SingletonComponent& operator=(const  SingletonComponent&) = delete;
	SingletonComponent(SingletonComponent&&) = delete;
	SingletonComponent& operator=(SingletonComponent&&) = delete;
	

	/// @brief �C���X�^���X���폜����
	void Destroy();
};

// ������
template <class T> T* SingletonComponent<T>::pInstance = nullptr;

template<class T>
inline T* SingletonComponent<T>::GetInstance()
{
	// ��������Ă��Ȃ���Ύ��Ԃ͖���
	return pInstance;
}

template<class T>
inline void SingletonComponent<T>::OnDestroy()
{
	Destroy();
}

template<class T>
inline void SingletonComponent<T>::Init()
{
	// ���ɐ�������Ă���Ȃ�
	if (pInstance)
	{
		HASHI_DEBUG_LOG(GetName() + "���ɃV���O���g���C���X�^���X�͐�������Ă��܂�");
		return;
	}

	// �������g����
	pInstance = static_cast<T*>(this);
}

template<class T>
inline void SingletonComponent<T>::Destroy()
{
	pInstance = nullptr;
}
