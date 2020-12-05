
#include "UEEnTTComponents.h"
#include "Kismet/GameplayStatics.h"

#include "ECSGameInstanceInterface.h"

void IECSComponentWrapper::RegisterComponentWithECS()
{
	if (EntityHandle == FEntity::NullEntity)
	{
		FRegistry& Registry = IECSGameInstanceInterface::GetRegistry(reinterpret_cast<UObject*>(this));
		EntityHandle = Registry.CreateEntity();			
	}
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
FActorPtrComponent::FActorPtrComponent(AActor* Actor)
{
	checkf(Actor, TEXT("The pointer must be valid"));
	Pointer = Actor;
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
UECS_BridgeComponent::UECS_BridgeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UECS_BridgeComponent::BeginPlay()
{
	Super::BeginPlay();

	// We are not calling RegisterComponentWithECS() on us (and we don't have code in that function), because we will call that function
	// on all our owner's components, which will include us
	IECSComponentWrapper::RegisterComponentWithECS();
	EntityHandle.AddComponent<FActorPtrComponent>(GetOwner());

	// Register all other ECS components from our owner
	for (auto Component : GetOwner()->GetComponents())
	{
		if(IECSComponentWrapper* WrapperComp = Cast<IECSComponentWrapper>(Component))
		{
			WrapperComp->EntityHandle = EntityHandle;
			WrapperComp->RegisterComponentWithECS();
		}
	}
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
UECS_TransformSyncComponent::UECS_TransformSyncComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UECS_TransformSyncComponent::RegisterComponentWithECS()
{
	EntityHandle.AddComponent<FTransform>(GetOwner()->GetActorTransform());
	EntityHandle.AddComponent<FTransformSync>(DefaultComponentParams);
}
