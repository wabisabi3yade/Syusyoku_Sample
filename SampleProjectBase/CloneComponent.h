#pragma once
#include "Component.h"

class CloneComponentBase
{
public:
    CloneComponentBase() {}
    virtual ~CloneComponentBase() {}

    virtual std::unique_ptr<Component> Clone() const = 0;
};

// Mixin ƒNƒ‰ƒX
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
