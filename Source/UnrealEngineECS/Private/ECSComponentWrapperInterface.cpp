
#include "ECSComponentWrapperInterface.h"


void UECSComponentWrapper::RegisterComponentWithECS()
{
	if (EntityHandle == FEntity::NullEntity)
	{
		UECSRegistry& Registry = UECSRegistry::GetRegistry();
		EntityHandle = Registry.Create();
	}
}

void UECSComponentWrapper::BeginPlay()
{
	Super::BeginPlay();
	RegisterComponentWithECS();
}
