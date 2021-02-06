#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UEEnTTEntity.h"
#include "ECSComponentWrapperInterface.generated.h"

/** Base class for components */
UCLASS()
class UNREALENGINEECS_API UECSComponentWrapper : public UActorComponent
{
	GENERATED_BODY()
    
public:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "ECS")
	virtual void UpdateECSComponent();
	
	/* Register this component with the ECS, by creating an entity and adding our Component to it.
	 * Must be called in BeginPlay() by child classes.
	 * You don't need to call the base implementation, because the UECS_BridgeComponent is setting EntityHandle for other components */
	virtual void RegisterComponentWithECS();
	
	/* The entity that we are representing. Only valid after BeginPlay() was called */
	FEntity EntityHandle;
};
