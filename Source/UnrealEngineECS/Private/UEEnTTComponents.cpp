
#include "UEEnTTComponents.h"
#include "Kismet/GameplayStatics.h"

#include "ECSGameInstanceInterface.h"


//////////////////////////////////////////////////
//////////////////////////////////////////////////
FActorPtrComponent::FActorPtrComponent(AActor* Actor)
{
	checkf(Actor, TEXT("The pointer must be valid"));
	Pointer = Actor;
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
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
		if (IECSComponentWrapper* WrapperComp = Cast<IECSComponentWrapper>(Component))
		{
			WrapperComp->EntityHandle = EntityHandle;
			WrapperComp->RegisterComponentWithECS();
		}
	}
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
void UECS_SyncTransformComponent::RegisterComponentWithECS()
{
	IECSComponentWrapper::RegisterComponentWithECS();
	
	EntityHandle.AddOrReplaceComponent<FTransform>(GetOwner()->GetActorTransform());

	UpdateECSComponent_Implementation();
}

void UECS_SyncTransformComponent::UpdateECSComponent_Implementation()
{
	if (SyncType == ESyncType::Disabled)
	{
		GetOwner()->GetRootComponent()->TransformUpdated.Remove(TransformChangedHandle);
		TransformChangedHandle.Reset();

		EntityHandle.RemoveComponentChecked<FSyncTransformToECS>();
		EntityHandle.RemoveComponentChecked<FSyncTransformToActor>();
		return;
	}

	if (SyncType == ESyncType::Actor_To_ECS)
	{
		EntityHandle.RemoveComponentChecked<FSyncTransformToActor>();		
		EntityHandle.AddOrReplaceComponent<FSyncTransformToECS>();

		if (!TransformChangedHandle.IsValid())
		{
			TransformChangedHandle = GetOwner()->GetRootComponent()->TransformUpdated.AddUObject(this,
				&UECS_SyncTransformComponent::OnRootComponentTransformChanged);
		}
	}
	else if (SyncType == ESyncType::ECS_To_Actor)
	{
		if (EntityHandle.RemoveComponentChecked<FSyncTransformToECS>())
		{
			GetOwner()->GetRootComponent()->TransformUpdated.Remove(TransformChangedHandle);
			TransformChangedHandle.Reset();
		}
		
		EntityHandle.AddOrReplaceComponent<FSyncTransformToActor>(DefaultValues);
	}
	else if (SyncType == ESyncType::BothWays)
	{
		EntityHandle.AddOrReplaceComponent<FSyncTransformToECS>();
		EntityHandle.AddOrReplaceComponent<FSyncTransformToActor>(DefaultValues);

		if (!TransformChangedHandle.IsValid())
		{
			TransformChangedHandle = GetOwner()->GetRootComponent()->TransformUpdated.AddUObject(this,
                &UECS_SyncTransformComponent::OnRootComponentTransformChanged);
		}
	}
}

//////////////////////////////////////////////////
void UECS_SyncTransformComponent::OnRootComponentTransformChanged(USceneComponent* UpdatedComponent,
																  EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	EntityHandle.GetComponent<FSyncTransformToECS>().bSyncTransform = true;
}
