
#include "UEEnTTEntity.h"

const FEntity FEntity::NullEntity = FEntity();


//////////////////////////////////////////////////
FEntity::FEntity(entt::entity Handle, entt::registry& Registry)
{
	this->EntityHandle = Handle;
	this->OwningRegistry = &Registry;
}

//////////////////////////////////////////////////
template <typename Component, typename... Args>
Component& FEntity::AddComponent(Args&&... args)
{
	checkf(!HasComponent<Component>(), TEXT("We already have a component with that class"));
	return OwningRegistry->emplace<Component>(EntityHandle, std::forward<Args>(args)...);
}

//////////////////////////////////////////////////
template <typename Component>
void FEntity::RemoveComponent()
{
	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
	OwningRegistry->remove<Component>(EntityHandle);
}

template <typename Component>
bool FEntity::RemoveComponentChecked()
{
	return OwningRegistry->remove_if_exists<Component>(EntityHandle) > 0;
}

//////////////////////////////////////////////////
template <typename Component>
Component& FEntity::GetComponent()
{
	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
	return OwningRegistry->get<Component>(EntityHandle);
}

template <typename Component>
Component& FEntity::GetComponent() const
{
	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
	return OwningRegistry->get<Component>(EntityHandle);
}

//////////////////////////////////////////////////
template <typename Component>
bool FEntity::HasComponent() const
{
	return OwningRegistry->has<Component>(EntityHandle);
}

template <typename... Component>
bool FEntity::HasAnyComponent() const
{
	return OwningRegistry->any<Component...>(EntityHandle);
}

template <typename... Component>
bool FEntity::HasAllComponent() const
{
	return OwningRegistry->has<Component...>(EntityHandle);
}

//////////////////////////////////////////////////
FEntity::operator bool() const
{
	return EntityHandle != entt::null;
}

bool FEntity::operator==(const FEntity& Other) const
{
	return Other.EntityHandle == EntityHandle;
}

bool FEntity::operator!=(const FEntity& Other) const
{
	return Other.EntityHandle != EntityHandle;
}

FEntity& FEntity::operator=(const entt::entity& OtherHandle)
{
	EntityHandle = OtherHandle;
	return *this;
}
