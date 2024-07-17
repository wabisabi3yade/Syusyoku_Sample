#pragma once

// �R���|�[�l���g�w�b�_�[
#include "ComponentDefine.h"

/// @brief �R���|�[�l���g�̃t�@�N�g���[���\�b�h
class ComponentFactory
{
public:

	/// @brief �R���|�[�l���g�쐬
	/// @tparam T �R���|�[�l���g�̌^
	/// @return �R���|�[�l���g�̃��j�[�N�|�C���^
	template<class T> static std::unique_ptr<T> Create();
};

/// @brief �R���|�[�l���g�쐬
/// @tparam T �R���|�[�l���g�̌^
/// @return �R���|�[�l���g�̃��j�[�N�|�C���^
template<class T>
inline std::unique_ptr<T> ComponentFactory::Create()
{
	std::unique_ptr<T> createComp = std::make_unique<T>();

	// �R���|�[�l���g���p�����Ă��Ȃ��Ȃ�
	if (dynamic_cast<Component*>(createComp.get()) == nullptr)
	{
		std::string compName = std::string(typeid(T).name());

		HASHI_DEBUG_LOG(compName + "��Component�ł͂���܂���");
		return nullptr;
	}

	return std::move(createComp);
}
