
#include "UEEnTTRegistry.h"
#include "UEEnTTEntity.h"
#include "Misc/LazySingleton.h"

FRegistry::~FRegistry()
{
	if (GetSingletonEntity().OwningRegistry == this)
	{
		TLazySingleton<FEntity>::TearDown();		
	}
}

//////////////////////////////////////////////////
FEntity FRegistry::CreateEntity()
{
	return FEntity(EnTTRegistry.create(), *this);
}

void FRegistry::CreateSingletonEntity()
{
	GetSingletonEntity() = CreateEntity();
}

//////////////////////////////////////////////////
FEntity& FRegistry::GetSingletonEntity()
{
	FEntity& Singleton = TLazySingleton<FEntity>::Get();
	checkf(Singleton != FEntity::NullEntity, TEXT("The singleton entity was not created! Call `CreateSingletonEntity()`"));	
	return Singleton;
}

//////////////////////////////////////////////////
template <typename ... Component, typename ... Exclude>
TView<TExclude<Exclude...>, Component...> FRegistry::View(TExclude<Exclude...>)
{
	return EnTTRegistry.view<Component..., Exclude...>(TExclude<Exclude...>());
}
