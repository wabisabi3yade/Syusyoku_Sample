#pragma once
#include "Component.h"
#include "ComponentConcept.h"

class ComponentRespawner_Base
{
public:
	ComponentRespawner_Base() {}
	virtual ~ComponentRespawner_Base() {}

	virtual std::unique_ptr<Component> Create() = 0;
};

template<HashiTaku::ComponentConcept T>
class ComponentRespawner : public ComponentRespawner_Base
{
public:
	ComponentRespawner() {}
	~ComponentRespawner() {}

	std::unique_ptr<Component> Create() override;
};

template<HashiTaku::ComponentConcept T>
inline std::unique_ptr<Component> ComponentRespawner<T>::Create()
{
	return std::make_unique<T>();
}
