#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UEEnTTEntity.h"
#include "ECSComponentWrapperInterface.generated.h"

UINTERFACE()
class UNREALENGINEECS_API UECSComponentWrapper : public UInterface
{
	GENERATED_BODY()
};

/** Interface for UActorComponents that want to have data in the ECS */
class UNREALENGINEECS_API IECSComponentWrapper
{
	GENERATED_BODY()
    
	public:
	/* Register this component with the ECS, by creating an entity and adding our Component to it.
	* Must be called in BeginPlay() by child classes.
	* You don't need to call the base implementation, because the UECS_BridgeComponent is setting EntityHandle for other components */
	virtual void RegisterComponentWithECS() = 0;
    
	/* The entity that we are representing. Only valid after BeginPlay() was called */
	FEntity EntityHandle;
};
