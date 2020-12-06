#pragma once
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
	IECSComponentWrapper();

	/** Update the ECS component with values (e.g. parameters) changed in the actor component */
	UFUNCTION(BlueprintNativeEvent, Category = "ECS")
	void UpdateECSComponent();
	
	/* Register this component with the ECS, by creating an entity and adding our Component to it.
	 * Must be called in BeginPlay() by child classes.
	 * You don't need to call the base implementation, because the UECS_BridgeComponent is setting EntityHandle for other components */
	virtual void RegisterComponentWithECS();
	
	/* The entity that we are representing. Only valid after BeginPlay() was called */
	FEntity EntityHandle;
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
/** Templated version of the IECSComponentWrapper interface */
template<typename Component>
class UNREALENGINEECS_API IECSTemplateComponentWrapper : public IECSComponentWrapper
{
public:
	virtual void RegisterComponentWithECS() override;
	virtual void UpdateECSComponent_Implementation() override;

	/** Overrides DefaultComponentParams and the ECS component with the new values */
	UFUNCTION(BlueprintCallable, Category = "ECS")
	virtual void UpdateECSComponent(const Component& NewValues);

	/** Returns a reference to the ECS component */
	UFUNCTION(BlueprintPure, Category = "ECS")
	Component& GetComponent() const;

protected:
	/* Default parameters for this component. When changed after BeginPlay(), you have to call UpdateECSComponent() in order to copy the
	 * new values to the ECS component. @see UpdateDefaultsFromECS() */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ECS")
	Component DefaultValues;
};
