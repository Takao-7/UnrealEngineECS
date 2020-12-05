// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"
#include "Modules/ModuleManager.h"

//////////////////////////////////////////////////
DECLARE_STATS_GROUP(TEXT("ECS"), STATGROUP_ECS, STATCAT_Advanced);

DECLARE_LOG_CATEGORY_EXTERN(LogUnrealECS, Log, Log);

//////////////////////////////////////////////////
class FUnrealEngineECSModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
