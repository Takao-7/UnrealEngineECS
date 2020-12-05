
#include "UEEnTTEntity.h"
#include "UEEnTTRegistry.h"


const FEntity FEntity::NullEntity = FEntity();

//////////////////////////////////////////////////
FEntity::FEntity(entt::entity Handle, FRegistry& Registry)
{
	this->EntityHandle = Handle;
	this->OwningRegistry = &Registry;
}

//////////////////////////////////////////////////
template <typename Component, typename ... Args>
Component& FEntity::AddComponent(Args&&... args)
{
	checkf(!HasComponent<Component>(), TEXT("We already have a component with that class"));
	return OwningRegistry->EnTTRegistry.emplace<Component>(EntityHandle, std::forward<Args>(args)...);
}

template <typename Component, typename ... Args>
Component& FEntity::AddComponent_Checked(bool bOverrideExistingComponent, Args&&... args)
{
	if (!HasComponent<Component>())
	{
		return OwningRegistry->EnTTRegistry.emplace<Component>(EntityHandle, std::forward<Args>(args)...);
	}
	else
	{
		Component& ExistingComponent = GetComponent<Component>();
		if (bOverrideExistingComponent)
		{
			ExistingComponent = Component(args...);
		}
		return ExistingComponent;
	}
}

//////////////////////////////////////////////////
template <typename Component>
void FEntity::RemoveComponent()
{
	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
	OwningRegistry->EnTTRegistry.remove<Component>(EntityHandle);
}

//////////////////////////////////////////////////
template <typename Component>
Component& FEntity::GetComponent()
{
	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
	return OwningRegistry->EnTTRegistry.get<Component>(EntityHandle);
}

template <typename Component>
Component& FEntity::GetComponent() const
{
	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
	return OwningRegistry->EnTTRegistry.get<Component>(EntityHandle);
}

template <typename ... Component>
decltype(auto) FEntity::GetComponents()
{
	checkf(HasAllComponent<Component...>(), TEXT("We don't have all components with these classes"));
	return OwningRegistry->EnTTRegistry.get<Component...>(EntityHandle);
}

//////////////////////////////////////////////////
template <typename Component>
bool FEntity::HasComponent() const
{
	return OwningRegistry->EnTTRegistry.has<Component>(EntityHandle);
}

template <typename ... Components>
bool FEntity::HasAnyComponent() const
{
	return OwningRegistry->EnTTRegistry.any<Components...>(EntityHandle);
}

template <typename ... Components>
bool FEntity::HasAllComponent() const
{
	return OwningRegistry->EnTTRegistry.has<Components...>(EntityHandle);
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
