
#include "ECSComponentWrapperInterface.h"
#include "ECSGameInstanceInterface.h"


void UECSComponentWrapper::RegisterComponentWithECS()
{
	if (EntityHandle == FEntity::NullEntity)
	{
		entt::registry& Registry = IECSGameInstanceInterface::GetRegistry(this);
		EntityHandle = Registry.create();
	}
}

void UECSComponentWrapper::BeginPlay()
{
	Super::BeginPlay();
	RegisterComponentWithECS();
}
