#pragma once
#include "Component.h"

class CloneComponentBase
{
public:
    CloneComponentBase() {}
    virtual ~CloneComponentBase() {}

    virtual std::unique_ptr<Component> Clone() const = 0;
};

// Mixin �N���X
template<class T>
class CloneComponent : public CloneComponentBase
{
public:
    CloneComponent() {}
    virtual ~CloneComponent() {}

    std::unique_ptr<Component> Clone() const override
    {
        return std::make_unique<T>(static_cast<const T&>(*this));
    }
};

namespace HashiTaku
{
    // �N���[���R���|�[�l���g�����p�����Ă��邩�̃R���Z�v�g
    template <typename T>
    concept ComponentConcept = std::is_base_of_v<CloneComponent<T>, T>;
}