// Copyright @Paul Larrass 2020


#include "ECSBlueprintLibrary.h"
#include "UEEnTTEntity.h"

bool UECSBlueprintLibrary::IsValid(const FEntity& Entity)
{
	return Entity != FEntity::NullEntity;
}
