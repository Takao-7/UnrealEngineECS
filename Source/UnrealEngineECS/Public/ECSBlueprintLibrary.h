// Copyright @Paul Larrass 2020

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UEEnTTEntity.h"
#include "ECSBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINEECS_API UECSBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "ECS Blueprint Library")
	static bool IsValid(const FEntity& Entity);
};
