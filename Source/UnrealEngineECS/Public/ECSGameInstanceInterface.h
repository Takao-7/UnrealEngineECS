// Copyright @Paul Larrass 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UEEnTTRegistry.h"
#include "ECSGameInstanceInterface.generated.h"


/** Interface for ECS */
class UNREALENGINEECS_API IECSGameInstanceInterface
{
public:
	/** Returns the registry. Will crash when the object is invalid or our game instance does not inherit from IECSGameInstanceInterface! */
	static FRegistry& GetRegistry(UObject* ObjectRef);

protected:
	FRegistry Registry;
};


/** Basic game instance which inherits from IECSGameInstanceInterface */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "ECS")
class UNREALENGINEECS_API UECSGameInstance : public UGameInstance, public IECSGameInstanceInterface
{
	GENERATED_BODY()

protected:
	/** Called when the game instance is started either normally or through PIE. */
	virtual void OnStart() override;
};
