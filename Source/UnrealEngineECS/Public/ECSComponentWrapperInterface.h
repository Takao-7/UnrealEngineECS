#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UEEnTTEntity.h"
#include "ECSComponentWrapperInterface.generated.h"

/** Base class for components */
UCLASS(Abstract)
class UNREALENGINEECS_API UECSComponentWrapper : public UActorComponent
{
	GENERATED_BODY()
    
public:
	virtual void BeginPlay() override;
	
	/**
	 * Register this component with the ECS, by creating an entity.
	 *
	 * After the Super:: call, add the ECS component to the entity here: EntityHandle.AddComponent<Component>();
	 * 
	 * This will be called in BeginPlay().
	 * You don't need to call the base implementation, because the UECS_BridgeComponent is setting EntityHandle for other components */
	virtual void RegisterComponentWithECS();

	/* The entity that we are representing. Only valid after RegisterComponentWithECS() was called */
	FEntity EntityHandle;
};
