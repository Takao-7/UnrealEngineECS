
#include "ECSRegistry.h"
#include "UEEnTTEntity.h"

//////////////////////////////////////////////////
inline FEntity IECSRegistryInterface::Create()
{
	FEntity NewEntity = FEntity(Registry.create(), *this);
	return NewEntity;
}

inline FEntity IECSRegistryInterface::Create(FEntity Hint)
{
	FEntity NewEntity = FEntity(Registry.create(Hint.EntityHandle), *this);
	return NewEntity;
}

//////////////////////////////////////////////////
inline void IECSRegistryInterface::Destroy(FEntity Entity)
{
	Registry.destroy(Entity.EntityHandle);
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
UECSRegistry* UECSRegistry::RegistryPtr = nullptr;

void UECSRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RegistryPtr = this;
}

void UECSRegistry::Deinitialize()
{
	RegistryPtr = nullptr;
}

//////////////////////////////////////////////////
UECSRegistry& UECSRegistry::GetRegistry()
{
	return *RegistryPtr;
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
void FECSObserver::Disconnect()
{
	Observer.disconnect();
}
