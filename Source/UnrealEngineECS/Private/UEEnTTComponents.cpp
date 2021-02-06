
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
void UECS_BridgeComponent::RegisterComponentWithECS()
{
	Super::RegisterComponentWithECS();

	EntityHandle.AddComponent<FActorPtrComponent>(GetOwner());

	// Register all other ECS components from our owner
	TInlineComponentArray<UECSComponentWrapper*> WrapperComponents (GetOwner());
	for (UECSComponentWrapper* Component : WrapperComponents)
	{
		if (Component != this)
		{
			Component->EntityHandle = EntityHandle;
			Component->RegisterComponentWithECS();			
		}
	}
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
void UECS_SyncTransformComponent::RegisterComponentWithECS()
{
	Super::RegisterComponentWithECS();	
	EntityHandle.AddOrReplaceComponent<FTransform>(GetOwner()->GetActorTransform());
	UpdateECSComponent();
}

void UECS_SyncTransformComponent::UpdateECSComponent()
{
	USceneComponent* OwnerRoot = GetOwner()->GetRootComponent();
	
	if (SyncType == ESyncType::Disabled)
	{
		OwnerRoot->TransformUpdated.Remove(TransformChangedHandle);
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
			TransformChangedHandle = OwnerRoot->TransformUpdated.AddUObject(this, &UECS_SyncTransformComponent::OnRootComponentTransformChanged);
		}
	}
	else if (SyncType == ESyncType::ECS_To_Actor)
	{
		if (EntityHandle.RemoveComponentChecked<FSyncTransformToECS>())
		{
			OwnerRoot->TransformUpdated.Remove(TransformChangedHandle);
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
			TransformChangedHandle = OwnerRoot->TransformUpdated.AddUObject(this, &UECS_SyncTransformComponent::OnRootComponentTransformChanged);
		}
	}
}

//////////////////////////////////////////////////
void UECS_SyncTransformComponent::OnRootComponentTransformChanged(USceneComponent* UpdatedComponent,
																  EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	EntityHandle.GetComponent<FSyncTransformToECS>().bSyncTransform = true;
}
