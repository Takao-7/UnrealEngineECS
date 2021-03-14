
#include "UEEnTTEntity.h"

const FEntity FEntity::NullEntity = FEntity();


//////////////////////////////////////////////////
FEntity::FEntity(entt::entity Handle, IECSRegistryInterface& Registry)
{
	this->EntityHandle = Handle;
	this->OwningRegistry = &Registry;
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
