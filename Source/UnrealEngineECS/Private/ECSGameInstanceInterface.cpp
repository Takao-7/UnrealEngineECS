// Copyright @Paul Larrass 2020


#include "ECSGameInstanceInterface.h"
#include "Engine/World.h"

FRegistry& IECSGameInstanceInterface::GetRegistry(UObject* ObjectRef)
{
	UWorld* World = ObjectRef->GetWorld();
	return reinterpret_cast<IECSGameInstanceInterface*>(World->GetGameInstance())->Registry;
}

//////////////////////////////////////////////////
void UECSGameInstance::OnStart()
{
	
}
